#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BOARD_SIZE 1000

#define SV_IMPLEMENTATION
#include "../sv.h"

void board_dump(uint64_t b[BOARD_SIZE][BOARD_SIZE]){
    for(uint64_t i=0; i<BOARD_SIZE; i++){
        for(uint64_t j=0; j<BOARD_SIZE; j++)
            printf("%ld", b[i][j]);
        printf("\n");
    }
}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    uint64_t board[BOARD_SIZE][BOARD_SIZE] = {0};
    while (input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        String_View l = sv_trim(sv_chop_by_delim(&line, '-'));
        sv_chop_left(&line, 2);
        uint64_t x1 = sv_to_u64(sv_chop_by_delim(&l, ','));
        uint64_t y1 = sv_to_u64(l);
        uint64_t x2 = sv_to_u64(sv_chop_by_delim(&line, ','));
        uint64_t y2 = sv_to_u64(line);

        if (x1 == x2){
            for(; y1 > y2; y1--)
                board[y1][x1]++;
            for(; y1 < y2; y1++)
                board[y1][x1]++;
            board[y1][x1]++;
        } else if (y1 == y2) {
            for(; x1 > x2; x1--)
                board[y1][x1]++;
            for(; x1 < x2; x1++)
                board[y1][x1]++;
            board[y1][x1]++;
        }
    }

    uint64_t count = 0;

    for(uint64_t i=0; i<BOARD_SIZE*BOARD_SIZE; i++){
        count += ((uint64_t*)board)[i] > 1;
    }

    return count;
}

size_t part_two() {
    String_View input = sv_slurp_file("input.txt");
    uint64_t board[BOARD_SIZE][BOARD_SIZE] = {0};
    while (input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        String_View l = sv_trim(sv_chop_by_delim(&line, '-'));
        sv_chop_left(&line, 2);
        uint64_t x1 = sv_to_u64(sv_chop_by_delim(&l, ','));
        uint64_t y1 = sv_to_u64(l);
        uint64_t x2 = sv_to_u64(sv_chop_by_delim(&line, ','));
        uint64_t y2 = sv_to_u64(line);

        uint64_t dx = x1 > x2 ? x1 - x2 : x2 - x1;
        uint64_t dy = y1 > y2 ? y1 - y2 : y2 - y1;
        int sx = x1 > x2 ? -1 : 1;
        int sy = y1 > y2 ? -1 : 1;
        
        if (dx == dy){
            board[y1][x1]++;
            while (x1 != x2){
                x1 += sx;
                y1 += sy;
                board[y1][x1]++;
            }
        }
        else if (dx == 0){
            board[y1][x1]++;
            while (y1 != y2){
                y1 += sy;
                board[y1][x1]++;
            }
        } else if (dy == 0) {
            board[y1][x1]++;
            while (x1 != x2){
                x1 += sx;
                board[y1][x1]++;
            }
        }
    }

    uint64_t count = 0;
    for(uint64_t i=0; i<BOARD_SIZE*BOARD_SIZE; i++){
        count += ((uint64_t*)board)[i] > 1;
    }

    return count;
}

int main() {
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
