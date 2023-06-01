const std = @import("std");

//const input = "abcdefgh";
//const input = "ghijklmn";
const input = "vzbxkghb";

const PasswordIterator = struct {
    buf: []u8,

    const Self = @This();

    pub fn next(self: Self) ?[]u8 {
        var i: usize = self.buf.len - 1;
        while (i >= 0) : (i -= 1) {
            self.buf[i] += 1;
            if (self.buf[i] > 'z') {
                self.buf[i] = 'a';
                if (i == 0) {
                    return null;
                }
            } else {
                return self.buf;
            }
        }
        return null;
    }
};

pub fn is_valid(pass: []const u8) bool {
    for (pass) |chr| {
        if (chr == 'i' and chr == 'o' and chr == 'l') {
            return false;
        }
    }

    outer: {
        var i: usize = 0;
        while (i < pass.len - 3) : (i += 1) {
            if (pass[i] + 1 == pass[i + 1] and pass[i] + 2 == pass[i + 2]) {
                break :outer;
            }
        }
        return false;
    }

    const has_repeat = outer: {
        var i: usize = 0;
        var repeats: u8 = 0;
        while (i < pass.len - 1) : (i += 1) {
            //std.debug.print("{c} == {c} = {any}\n", .{ pass[i], pass[i + 1], pass[i] == pass[i + 1] });
            if (pass[i] == pass[i + 1]) {
                if (repeats > 0) {
                    break :outer true;
                } else {
                    repeats += 1;
                    i += 1;
                }
            }
        }
        break :outer false;
    };

    return has_repeat;
}

pub fn main() void {
    var inp = input.*;
    var iterator = PasswordIterator{ .buf = &inp };
    while (iterator.next()) |pass| {
        if (is_valid(pass)) {
            break;
        }
    }
    std.debug.print("Part One: {s}\n", .{inp});
    while (iterator.next()) |pass| {
        if (is_valid(pass)) {
            break;
        }
    }
    std.debug.print("Part Two: {s}\n", .{inp});
}
