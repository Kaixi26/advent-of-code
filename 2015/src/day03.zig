const std = @import("std");

const filename = "inputs/03.txt";
const dir = std.fs.cwd();

const Pos = struct {
    x: i16 = 0,
    y: i16 = 0,

    const Self = @This();

    pub const context = struct {
        pub fn hash(_: @This(), key: Pos) u64 {
            return @as(u64, 0) | @as(u64, @bitCast(u16, key.x)) | (@as(u64, @bitCast(u16, key.y)) << 16);
        }
        pub fn eql(_: @This(), pos: Pos, pos2: Pos) bool {
            return pos.x == pos2.x and pos.y == pos2.y;
        }
    };

    pub fn move(self: *Self, direction: u8) void {
        switch (direction) {
            '<' => {
                self.x -= 1;
            },
            '>' => {
                self.x += 1;
            },
            '^' => {
                self.y += 1;
            },
            'v' => {
                self.y -= 1;
            },
            '\n' => {},
            else => {
                std.debug.print("Unsupported direction `{}`\n", .{direction});
                unreachable;
            },
        }
    }
};

pub fn main() !void {
    var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = general_purpose_allocator.deinit();
    var gpa = general_purpose_allocator.allocator();

    const file = try dir.openFile(filename, std.fs.File.OpenFlags{});
    defer file.close();

    var buffer: [10240]u8 = undefined;
    const read = try file.readAll(&buffer);
    const input = buffer[0..read];

    var visited = std.HashMap(Pos, void, Pos.context, 80).init(gpa);
    defer visited.deinit();

    {
        var pos: Pos = Pos{};

        try visited.put(pos, undefined);
        for (input) |chr| {
            pos.move(chr);
            try visited.put(pos, undefined);
        }

        std.debug.print("Part One: {}\n", .{visited.count()});
    }

    visited.clearRetainingCapacity();

    {
        var santa_pos: Pos = Pos{};
        var robot_pos: Pos = Pos{};
        var santa_turn: bool = true;

        try visited.put(santa_pos, undefined);
        for (input) |chr| {
            if (santa_turn) {
                santa_pos.move(chr);
                try visited.put(santa_pos, undefined);
            } else {
                robot_pos.move(chr);
                try visited.put(robot_pos, undefined);
            }
            santa_turn = !santa_turn;
        }

        std.debug.print("Part Two: {}\n", .{visited.count()});
    }
}
