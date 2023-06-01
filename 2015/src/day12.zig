const std = @import("std");
const cwd = std.fs.cwd();

const filename = "inputs/12.txt";

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

pub fn is_red(object: std.json.Value) bool {
    switch (object) {
        .String => {
            return std.mem.eql(u8, object.String, "red");
        },
        else => {
            return false;
        },
    }
}

pub fn calculate_non_red(object: std.json.Value) i64 {
    switch (object) {
        .Integer => {
            return object.Integer;
        },
        .Array => {
            var sum: i64 = 0;
            for (object.Array.items) |item| {
                sum += calculate_non_red(item);
            }
            return sum;
        },
        .Object => {
            var sum: i64 = 0;
            var iter = object.Object.iterator();
            while (iter.next()) |entry| {
                if (is_red(entry.value_ptr.*)) {
                    return 0;
                }
                sum += calculate_non_red(entry.value_ptr.*);
            }
            return sum;
        },
        else => {
            return 0;
        },
    }
}

pub fn main() !void {
    const file = try cwd.openFile(filename, std.fs.File.OpenFlags{});
    defer file.close();

    var buf: [102400]u8 = undefined;

    const read = try file.readAll(&buf);
    std.debug.assert(read < buf.len);

    {
        var unread: []u8 = buf[0..read];

        var number_sum: i64 = 0;
        while (unread.len > 0) {
            const prefix = drop_while(&unread, is_not_digit);
            const num_str = drop_while(&unread, std.ascii.isDigit);
            const num: i64 = std.fmt.parseInt(i64, num_str, 10) catch 0;
            if (prefix[prefix.len - 1] == '-') {
                number_sum -= num;
            } else {
                number_sum += num;
            }
        }

        std.debug.print("Part One: {}\n", .{number_sum});
    }

    {
        var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};
        defer _ = general_purpose_allocator.deinit();
        var gpa = general_purpose_allocator.allocator();

        var parser = std.json.Parser.init(gpa, false);
        defer parser.deinit();
        var tree = try parser.parse(buf[0..read]);
        defer tree.deinit();

        std.debug.print("Part Two: {}\n", .{calculate_non_red(tree.root)});
    }
}
