const std = @import("std");
const File = std.fs.File;

const filename = "inputs/05.txt";
const cwd = std.fs.cwd();

pub fn isNice(str: []u8) bool {
    const vowels = "aeiou";
    var vowel_count: u8 = 0;
    for (str) |chr| {
        for (vowels) |vowel| {
            if (chr == vowel) {
                vowel_count += 1;
                break;
            }
        }
    }
    if (vowel_count < 3) {
        return false;
    }

    var duplicate_sequence = false;
    for (str[0 .. str.len - 1]) |_, i| {
        if (str[i] == str[i + 1]) {
            duplicate_sequence = true;
            break;
        }
    }
    if (!duplicate_sequence) {
        return false;
    }

    const forbiddens = [_][2]u8{
        [_]u8{ 'a', 'b' },
        [_]u8{ 'c', 'd' },
        [_]u8{ 'p', 'q' },
        [_]u8{ 'x', 'y' },
    };
    for (str[0 .. str.len - 1]) |_, i| {
        for (forbiddens) |forbidden| {
            if (str[i] == forbidden[0] and str[i + 1] == forbidden[1]) {
                return false;
            }
        }
    }

    return true;
}

pub fn isNice2(str: []u8) bool {
    var repeated_pairs = false;
    {
        var i: usize = 0;
        while (i < str.len - 1) {
            var j: usize = i + 2;
            while (j < str.len - 1) {
                if (str[i] == str[j] and str[i + 1] == str[j + 1]) {
                    repeated_pairs = true;
                }
                j += 1;
            }
            i += 1;
        }
    }
    if (!repeated_pairs) {
        return false;
    }

    for (str[0 .. str.len - 2]) |_, i| {
        if (str[i] == str[i + 2]) {
            return true;
        }
    }

    return false;
}

pub fn main() !void {
    var file = try cwd.openFile(filename, File.OpenFlags{});
    defer file.close();

    var nice_strings: u16 = 0;
    var nice_strings2: u16 = 0;
    var reader: File.Reader = file.reader();
    var buf: [256]u8 = undefined;
    while (try reader.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        if (isNice(line)) {
            nice_strings += 1;
        }
        if (isNice2(line)) {
            nice_strings2 += 1;
        }
    }

    std.debug.print("Part One: {}\n", .{nice_strings});
    std.debug.print("Part Two: {}\n", .{nice_strings2});
}
