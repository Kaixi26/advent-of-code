const std = @import("std");
const assert = std.debug.assert;

const filename = "inputs/02.txt";
const dir = std.fs.cwd();

const Box = struct {
    w: u16,
    h: u16,
    l: u16,

    pub fn fromLine(line: []u8) std.fmt.ParseIntError!Box {
        var indices: [3]usize = [_]usize{ 0, 0, 0 };
        var cursor: usize = 1;
        for (line) |chr, i| {
            if (chr == 'x') {
                assert(cursor < 3);
                indices[cursor] = i;
                cursor += 1;
            }
        }
        return Box{
            .w = try std.fmt.parseInt(u16, line[indices[0]..indices[1]], 10),
            .h = try std.fmt.parseInt(u16, line[indices[1] + 1 .. indices[2]], 10),
            .l = try std.fmt.parseInt(u16, line[indices[2] + 1 .. line.len], 10),
        };
    }

    pub fn cost(self: Box) u32 {
        const area = 2 * self.l * self.w + 2 * self.w * self.h + 2 * self.h * self.l;
        const min_side = std.math.min3(self.l * self.w, self.w * self.h, self.h * self.l);
        return area + min_side;
    }

    pub fn cost2(self: Box) u32 {
        const volume = self.l * self.w * self.h;
        var xs = [_]u16{ self.l, self.w, self.h };
        std.sort.sort(u16, &xs, {}, std.sort.asc(u16));
        return volume + (xs[0] + xs[1]) * 2;
    }
};

pub fn main() !void {
    var file = try dir.openFile(filename, std.fs.File.OpenFlags{});
    defer file.close();

    var total_cost: u64 = 0;
    var total_cost2: u64 = 0;
    {
        var buffer: [1024]u8 = undefined;
        var reader = file.reader();
        while (true) {
            if (try reader.readUntilDelimiterOrEof(&buffer, '\n')) |line| {
                const box = try Box.fromLine(line);
                std.debug.print("{any} = {}\n", .{ box, box.cost() });
                total_cost += box.cost();
                total_cost2 += box.cost2();
            } else {
                break;
            }
        }
    }
    std.debug.print("Part One: {}\n", .{total_cost});
    std.debug.print("Part Two: {}\n", .{total_cost2});
}
