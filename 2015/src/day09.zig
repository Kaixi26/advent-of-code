const std = @import("std");
const Allocator = std.mem.Allocator;

const filename = "inputs/09.txt";
const cwd = std.fs.cwd();

fn not_digit(chr: u8) bool {
    return !std.ascii.isDigit(chr);
}

fn not_whitespace(chr: u8) bool {
    return !std.ascii.isSpace(chr);
}

// lol https://en.wikipedia.org/wiki/Heap%27s_algorithm
pub fn Permutations(comptime T: type) type {
    return struct {
        allocator: Allocator,
        buf: []T,
        c: []usize,
        i: usize = 0,

        const Self = @This();

        pub fn iterator(buf: []T, allocator: Allocator) !Self {
            var c = try allocator.alloc(usize, buf.len);
            std.mem.set(usize, c, 0);
            return .{ .buf = buf, .allocator = allocator, .c = c };
        }

        pub fn deinit(self: Self) void {
            self.allocator.free(self.c);
        }

        pub fn next(self: *Self) ?[]T {
            if (self.i == 0) {
                self.i = 1;
                return self.buf;
            }

            var result: ?[]T = null;
            while (result == null) {
                if (self.i >= self.buf.len) {
                    return null;
                }

                if (self.c[self.i] < self.i) {
                    if (self.i % 2 == 0) {
                        std.mem.swap(T, &self.buf[0], &self.buf[self.i]);
                    } else {
                        std.mem.swap(T, &self.buf[self.c[self.i]], &self.buf[self.i]);
                    }
                    result = self.buf;
                    self.c[self.i] += 1;
                    self.i = 1;
                } else {
                    self.c[self.i] = 0;
                    self.i += 1;
                }
            }
            return result;
        }
    };
}

pub fn drop_while(str: *[]u8, comptime predicate: fn (u8) bool) []u8 {
    var i: usize = 0;
    while (i < str.len and predicate(str.*[i])) : (i += 1) {}
    const result = str.*[0..i];
    str.* = str.*[i..];
    return result;
}

const Graph = struct {
    allocator: Allocator,
    vertices: std.ArrayList([]u8),
    edges: std.HashMap(Edge, u16, EdgesContext, 80),

    const Edge = struct {
        from: u32,
        to: u32,
    };

    const Self = @This();
    const EdgesContext = struct {
        pub fn hash(_: @This(), edge: Edge) u64 {
            return (@as(u64, edge.from) << 32) | edge.to;
        }

        pub fn eql(_: @This(), e1: Edge, e2: Edge) bool {
            return e1.from == e2.from and e1.to == e2.to;
        }
    };

    pub fn init(allocator: Allocator) Self {
        return .{
            .allocator = allocator,
            .vertices = std.ArrayList([]u8).init(allocator),
            .edges = std.HashMap(Edge, u16, EdgesContext, 80).init(allocator),
        };
    }

    pub fn deinit(self: *Self) void {
        for (self.vertices.items) |item| {
            self.allocator.free(item);
        }
        self.vertices.deinit();
        self.edges.deinit();
    }

    pub fn get_or_append_vertex(self: *Self, name: []u8) !u32 {
        for (self.vertices.items) |item, i| {
            if (std.mem.eql(u8, item, name)) {
                return @intCast(u32, i);
            }
        }
        var internal_name: []u8 = try self.allocator.alloc(u8, name.len);
        std.mem.copy(u8, internal_name, name);
        try self.vertices.append(internal_name);
        return @intCast(u32, self.vertices.items.len - 1);
    }

    pub fn put_edge(self: *Self, from: []u8, to: []u8, cost: u16) !void {
        const edge: Edge = .{
            .from = try self.get_or_append_vertex(from),
            .to = try self.get_or_append_vertex(to),
        };
        try self.edges.put(edge, cost);
    }

    pub fn path_cost(self: Self, route: []u32) u32 {
        var total_cost: u32 = 0;
        var i: usize = 0;
        while (i < route.len - 1) : (i += 1) {
            const from = route[i];
            const to = route[i + 1];
            const cost = self.edges.get(.{ .from = from, .to = to }).?;
            total_cost += cost;
        }
        return total_cost;
    }

    pub fn path_lengths(self: Self) !(struct { shortest: u32, longest: u32 }) {
        var shortest_path = try self.allocator.alloc(u32, self.vertices.items.len);
        defer self.allocator.free(shortest_path);

        var path = try self.allocator.alloc(u32, self.vertices.items.len);
        defer self.allocator.free(path);
        for (path) |_, i| {
            path[i] = @intCast(u32, i);
        }

        var paths = try Permutations(u32).iterator(path, self.allocator);
        defer paths.deinit();

        var shortest_path_cost: u32 = 0xFFFFFFFF;
        var longest_path_cost: u32 = 0;
        while (paths.next()) |path_permutation| {
            shortest_path_cost = @min(shortest_path_cost, self.path_cost(path_permutation));
            longest_path_cost = @max(longest_path_cost, self.path_cost(path_permutation));
            //std.debug.print("{any} {}\n", .{ path_permutation, self.path_cost(path_permutation) });
        }

        return .{
            .shortest = shortest_path_cost,
            .longest = longest_path_cost,
        };
    }

    pub fn format(
        self: Graph,
        comptime fmt: []const u8,
        options: std.fmt.FormatOptions,
        writer: anytype,
    ) !void {
        _ = fmt;
        _ = options;

        try writer.print("Graph {{ ", .{});

        for (self.vertices.items) |item, i| {
            if (i > 0) {
                try writer.print(", ", .{});
            }
            try writer.print("{s}: {}", .{ item, i });
        }

        try writer.print("; ", .{});
        var iter = self.edges.iterator();
        while (iter.next()) |edge| {
            if (iter.index > 1) {
                try writer.print(", ", .{});
            }
            try writer.print("{} -> {} ({})", .{ edge.key_ptr.from, edge.key_ptr.to, edge.value_ptr.* });
        }

        try writer.print(" }}", .{});
    }
};

pub fn main() !void {
    var file = try cwd.openFile(filename, std.fs.File.OpenFlags{});
    defer file.close();

    var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = general_purpose_allocator.deinit();
    var gpa = general_purpose_allocator.allocator();

    var graph = Graph.init(gpa);
    defer graph.deinit();

    var foo: [4]u8 = undefined;
    std.mem.copy(u8, &foo, "abcd");

    const reader = file.reader();
    var buf: [1024]u8 = undefined;
    while (try reader.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        var unread = line;
        const from_name = drop_while(&unread, not_whitespace);
        _ = drop_while(&unread, std.ascii.isSpace);
        _ = drop_while(&unread, not_whitespace);
        _ = drop_while(&unread, std.ascii.isSpace);
        const to_name = drop_while(&unread, not_whitespace);
        _ = drop_while(&unread, not_digit);
        const cost = try std.fmt.parseInt(u16, unread, 10);
        //std.debug.print("{s}_{s}_{}\n", .{ from_name, to_name, cost });
        try graph.put_edge(from_name, to_name, cost);
        try graph.put_edge(to_name, from_name, cost);
        //std.debug.print("{s}\n", .{line});
    }

    //std.debug.print("{s}\n", .{graph});
    std.debug.print("Part One: {}\n", .{(try graph.path_lengths()).shortest});
    std.debug.print("Part Two: {}\n", .{(try graph.path_lengths()).longest});
}
