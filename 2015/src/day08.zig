const std = @import("std");

const filename = "inputs/08.txt";
const cwd = std.fs.cwd();

pub fn string_len(str: []u8) u64 {
    std.debug.assert(str[0] == '"');
    std.debug.assert(str[str.len - 1] == '"');
    std.debug.assert(str.len >= 2);

    var result: u64 = 0;
    var i: usize = 1;
    while (i < str.len - 1) : (i += 1) {
        switch (str[i]) {
            '\\' => {
                switch (str[i + 1]) {
                    'x' => i += 3,
                    else => i += 1,
                }
            },
            else => {},
        }
        result += 1;
    }

    return result;
}

pub fn reencoded_len(str: []u8) u64 {
    var result: u64 = str.len + 2;
    for (str) |chr| {
        switch (chr) {
            '"', '\\' => result += 1,
            else => {},
        }
    }

    return result;
}

pub fn main() !void {
    const file = try cwd.openFile(filename, std.fs.File.OpenFlags{});
    defer file.close();

    var total_repr_len: u64 = 0;
    var total_size: u64 = 0;
    var total_reencoded_len: u64 = 0;

    const reader = file.reader();
    var buf: [1024]u8 = undefined;
    while (try reader.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        total_repr_len += line.len;
        total_size += string_len(line);
        total_reencoded_len += reencoded_len(line);
        //std.debug.print("{s} {}\n", .{ line, string_len(line) });
    }

    std.debug.print("Part One: {} - {} = {}\n", .{ total_repr_len, total_size, total_repr_len - total_size });
    std.debug.print("Part Two: {} - {} = {}\n", .{ total_reencoded_len, total_repr_len, total_reencoded_len - total_repr_len });
}
