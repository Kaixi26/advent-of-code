const std = @import("std");
const assert = std.debug.assert;

pub const filename = "inputs/1.txt";
pub const dir = std.fs.cwd();

pub fn main() !void {
    const file = try dir.openFile(filename, std.fs.File.OpenFlags{});
    defer file.close();

    var buffer: [10240]u8 = undefined;
    const len = try file.readAll(&buffer);
    const input = buffer[0..len];
    assert(len < buffer.len);

    var floor: i32 = 0;
    for (input) |chr| {
        switch (chr) {
            '(' => {
                floor += 1;
            },
            ')' => {
                floor -= 1;
            },
            else => {},
        }
    }
    std.debug.print("Part One: {}\n", .{floor});

    floor = 0;
    var position: usize = undefined;
    for (input) |chr, i| {
        if (floor == -1) {
            position = i;
            break;
        }
        switch (chr) {
            '(' => {
                floor += 1;
            },
            ')' => {
                floor -= 1;
            },
            else => {},
        }
    }

    std.debug.print("Part Two: {}\n", .{position});
}
