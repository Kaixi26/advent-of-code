const std = @import("std");
const Md5 = std.crypto.hash.Md5;

//const input = "abcdef";
const input = "bgvyzdsv";

pub fn main() !void {
    var in: [16]u8 = undefined;
    var out: [16]u8 = undefined;
    var i: u32 = 0;
    var opt_result: ?u32 = null;
    var opt_result2: ?u32 = null;
    while (opt_result2 == null) {
        const in_slice = try std.fmt.bufPrint(&in, "{s}{}", .{ input, i });
        Md5.hash(in_slice, &out, Md5.Options{});

        if (opt_result == null and out[0] == 0x00 and out[1] == 0x00 and (out[2] & 0xF0) == 0x00) {
            opt_result = i;
        }
        if (out[0] == 0x00 and out[1] == 0x00 and out[2] == 0x00) {
            opt_result2 = i;
        }

        i += 1;

        //std.debug.print("{s}\n", .{in_slice});
        //std.debug.print("{any}\n", .{out});
    }

    if (opt_result) |result| {
        std.debug.print("Part One: {}\n", .{result});
    }
    if (opt_result2) |result2| {
        std.debug.print("Part Two: {}\n", .{result2});
    }
}
