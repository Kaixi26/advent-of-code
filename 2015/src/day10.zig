const std = @import("std");

//const input: [1]u8 = [_]u8{1};
const input: [10]u8 = [_]u8{ 1, 1, 1, 3, 1, 2, 2, 1, 1, 3 };

pub fn main() !void {
    var general_purpose_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = general_purpose_allocator.deinit();
    var gpa = general_purpose_allocator.allocator();

    var active = std.ArrayList(u8).init(gpa);
    defer active.deinit();

    for (input) |n| {
        try active.append(n);
    }

    var scrathpad = std.ArrayList(u8).init(gpa);
    defer scrathpad.deinit();

    var iteration: usize = 0;
    // Zig bruteforce too strong
    while (iteration < 50) : (iteration += 1) {
        if (iteration == 40) {
            std.debug.print("Part One: {}\n", .{active.items.len});
        }

        var count: u8 = 1;
        scrathpad.clearRetainingCapacity();
        for (active.items) |_, i| {
            if (i == active.items.len - 1 or active.items[i] != active.items[i + 1]) {
                try scrathpad.append(count);
                try scrathpad.append(active.items[i]);
                count = 1;
            } else {
                count += 1;
            }
        }
        std.mem.swap(@TypeOf(active), &active, &scrathpad);
        //std.debug.print("{any}\n", .{active.items});
    }

    std.debug.print("Part Two: {}\n", .{active.items.len});
}
