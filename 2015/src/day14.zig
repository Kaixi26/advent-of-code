const std = @import("std");
const cwd = std.fs.cwd();

const filename = "inputs/14.txt";

pub fn is_not_space(chr: u8) bool {
    return !std.ascii.isWhitespace(chr);
}
pub fn is_not_digit(chr: u8) bool {
    return !std.ascii.isDigit(chr);
}

pub fn drop_while(buf: *[]u8, comptime predicate: fn (u8) bool) []u8 {
    var i: usize = 0;
    while (i < buf.len and predicate(buf.*[i])) : (i += 1) {}
    const result = buf.*[0..i];
    buf.* = buf.*[i..];
    return result;
}

const Reindeer = struct {
    name: []u8,
    speed: u8,
    stamina: u8,
    rest: u8,
    steps: u64 = 0,
    distance: u64 = 0,

    const Self = @This();

    pub fn format(
        self: Self,
        comptime fmt: []const u8,
        options: std.fmt.FormatOptions,
        writer: anytype,
    ) !void {
        _ = fmt;
        _ = options;

        try writer.print("Reindeer {{ {s}, {} km/s * {}, {} s }}", .{ self.name, self.speed, self.stamina, self.rest });
    }

    pub fn step(self: *Self) void {
        const current_step = self.steps % (self.stamina + self.rest);
        if (current_step < self.stamina) {
            self.distance += self.speed;
        }
        self.steps += 1;
    }

    pub fn compute(self: Self, secs: u64) u64 {
        var i: usize = 0;
        var distance: u64 = 0;
        while (i < secs) : (i += 1) {
            const current_step = i % (self.stamina + self.rest);
            if (current_step < self.stamina) {
                distance += self.speed;
            }
        }
        return distance;
    }
};

const Foo = struct {
    i: usize,
    j: usize,
    allocator: std.mem.Allocator,
};

pub fn main() !void {
    var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = general_purpose_allocator.deinit();
    var gpa = general_purpose_allocator.allocator();

    var file = try cwd.openFile(filename, std.fs.File.OpenFlags{});
    defer file.close();

    {
        var arena = std.heap.ArenaAllocator.init(gpa);
        defer arena.deinit();

        var reindeers = std.ArrayList(Reindeer).init(arena.allocator());
        defer reindeers.deinit();

        var file_reader = file.reader();
        while (try file_reader.readUntilDelimiterOrEofAlloc(arena.allocator(), '\n', 6969)) |line| {
            var unread = line;
            var name = drop_while(&unread, is_not_space);
            _ = drop_while(&unread, is_not_digit);
            var speed = try std.fmt.parseInt(u8, drop_while(&unread, std.ascii.isDigit), 10);
            _ = drop_while(&unread, is_not_digit);
            var stamina = try std.fmt.parseInt(u8, drop_while(&unread, std.ascii.isDigit), 10);
            _ = drop_while(&unread, is_not_digit);
            var rest = try std.fmt.parseInt(u8, drop_while(&unread, std.ascii.isDigit), 10);
            try reindeers.append(.{ .name = name, .speed = speed, .stamina = stamina, .rest = rest });
        }

        {
            var max_distance: u64 = 0;
            for (reindeers.items) |reindeer| {
                max_distance = @max(max_distance, reindeer.compute(2503));
            }
            std.debug.print("Part One: {}\n", .{max_distance});
        }

        {
            var points = std.ArrayList(u64).init(arena.allocator());
            defer points.deinit();
            for (reindeers.items) |_| {
                try points.append(0);
            }
            var i: usize = 0;
            while (i < 2503) : (i += 1) {
                var round_winner: usize = undefined;
                var round_winner_distance: u64 = 0;
                for (reindeers.items, 0..) |*reindeer, ri| {
                    reindeer.step();
                    if (reindeer.distance > round_winner_distance) {
                        round_winner = ri;
                        round_winner_distance = reindeer.distance;
                    }
                }
                points.items[round_winner] += 1;
            }

            var max_points: u64 = 0;
            for (points.items) |point| {
                max_points = @max(max_points, point);
            }

            std.debug.print("Part Two: {}\n", .{max_points});
        }
    }
}
