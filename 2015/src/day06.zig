const std = @import("std");
const StringView = @import("string_view");

const filename = "inputs/06.txt";
const cwd = std.fs.cwd();

fn is_not_digit(chr: u8) bool {
    return !std.ascii.isDigit(chr);
}

fn take_while(str: *[]u8, comptime predicate: fn (u8) bool) []u8 {
    var i: usize = 0;
    while (i < str.len and predicate(str.*[i])) {
        i += 1;
    }
    const result = str.*[0..i];
    str.* = str.*[i..str.len];
    return result;
}

const Op = enum { Toggle, On, Off };
const Coord = struct { x: u16, y: u16 };
const Instruction = struct {
    from: Coord,
    to: Coord,
    op: Op,

    pub const Self = @This();

    pub fn init(x1: u16, y1: u16, x2: u16, y2: u16, op: Op) Self {
        return .{ .from = .{ .x = x1, .y = y1 }, .to = .{ .x = x2, .y = y2 }, .op = op };
    }

    pub fn from_line(line: []u8) !Self {
        const parseInt = std.fmt.parseInt;

        var unread = line;
        const operation = take_while(&unread, is_not_digit);
        const x1 = try parseInt(u16, take_while(&unread, std.ascii.isDigit), 10);
        _ = take_while(&unread, is_not_digit);
        const y1 = try parseInt(u16, take_while(&unread, std.ascii.isDigit), 10);
        _ = take_while(&unread, is_not_digit);
        const x2 = try parseInt(u16, take_while(&unread, std.ascii.isDigit), 10);
        _ = take_while(&unread, is_not_digit);
        const y2 = try parseInt(u16, take_while(&unread, std.ascii.isDigit), 10);

        var op: Op = undefined;
        if (std.mem.startsWith(u8, operation, "turn on")) {
            op = Op.On;
        } else if (std.mem.startsWith(u8, operation, "turn off")) {
            op = Op.Off;
        } else if (std.mem.startsWith(u8, operation, "toggle")) {
            op = Op.Toggle;
        } else {
            unreachable;
        }

        return init(x1, y1, x2, y2, op);
    }
};

const Board = struct {
    tiles: []bool,
    brightness: []u16,
    allocator: std.mem.Allocator,

    const Self = @This();

    pub fn init(allocator: std.mem.Allocator) !Self {
        const tiles = try allocator.alloc(bool, 1000 * 1000);
        std.mem.set(bool, tiles, false);
        const brightness = try allocator.alloc(u16, 1000 * 1000);
        std.mem.set(u16, brightness, 0);
        return .{
            .tiles = tiles,
            .brightness = brightness,
            .allocator = allocator,
        };
    }

    inline fn index(x: u16, y: u16) usize {
        return @as(u32, x) + @as(u32, y) * 1000;
    }

    inline fn set(self: Self, x: u16, y: u16, b: bool) void {
        self.tiles[index(x, y)] = b;
        if (b) {
            self.brightness[index(x, y)] += 1;
        } else if (self.brightness[index(x, y)] > 0) {
            self.brightness[index(x, y)] -= 1;
        }
    }

    inline fn toggle(self: Self, x: u16, y: u16) void {
        self.tiles[index(x, y)] = !self.tiles[index(x, y)];
        self.brightness[index(x, y)] += 2;
    }

    inline fn get(self: Self, x: u16, y: u16) bool {
        return self.tiles[index(x, y)];
    }

    fn execute_instruction(self: Self, instruction: Instruction) void {
        var x: u16 = instruction.from.x;
        while (x <= instruction.to.x) {
            var y: u16 = instruction.from.y;
            while (y <= instruction.to.y) {
                switch (instruction.op) {
                    Op.Toggle => self.toggle(x, y),
                    Op.On => self.set(x, y, true),
                    Op.Off => self.set(x, y, false),
                }
                y += 1;
            }
            x += 1;
        }
    }

    pub fn lit(self: Self) u32 {
        var result: u32 = 0;
        for (self.tiles) |tile| {
            if (tile) {
                result += 1;
            }
        }
        return result;
    }

    pub fn lit2(self: Self) u64 {
        var result: u64 = 0;
        for (self.brightness) |v| {
            result += v;
        }
        return result;
    }

    pub fn deinit(self: Self) void {
        self.allocator.free(self.tiles);
        self.allocator.free(self.brightness);
    }
};

pub fn main() !void {
    var file = try cwd.openFile(filename, std.fs.File.OpenFlags{});
    defer file.close();

    var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = general_purpose_allocator.deinit();
    var gpa = general_purpose_allocator.allocator();

    var board = try Board.init(gpa);
    defer board.deinit();

    var reader = file.reader();
    var buf: [1024]u8 = undefined;
    while (try reader.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        const instruction = try Instruction.from_line(line);
        board.execute_instruction(instruction);
        //std.debug.print("{any}\n", .{try Instruction.from_line(line)});
        //std.debug.print("{s}\n", .{line});
    }

    std.debug.print("Part One: {}\n", .{board.lit()});
    std.debug.print("Part Two: {}\n", .{board.lit2()});
}
