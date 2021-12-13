#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SV_IMPLEMENTATION
#include "../sv.h"

#define MATRIX_SIZE (2000)

void fold_x(bool grid[MATRIX_SIZE][MATRIX_SIZE], size_t fx){
    for(size_t y=0; y<MATRIX_SIZE; y++)
        for(size_t x=0; x<=fx && x<MATRIX_SIZE; x++)
            grid[y][fx-x] = grid[y][fx-x] || grid[y][fx+x];
    for(size_t y=0; y<MATRIX_SIZE; y++)
        for(size_t x=fx; x<MATRIX_SIZE; x++)
            grid[y][x] = false;
}

void fold_y(bool grid[MATRIX_SIZE][MATRIX_SIZE], size_t fy){
    for(size_t y=0; y<=fy && y+fy<MATRIX_SIZE; y++)
        for(size_t x=0; x<MATRIX_SIZE; x++)
            grid[fy-y][x] = grid[fy-y][x] || grid[fy+y][x];
    for(size_t y=fy; y<MATRIX_SIZE; y++)
        for(size_t x=0; x<MATRIX_SIZE; x++)
            grid[y][x] = false;
}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    String_View line = sv_chop_by_delim(&input, '\n');
    bool grid[MATRIX_SIZE][MATRIX_SIZE] = {0};
    do {
        uint64_t x = sv_to_u64(sv_chop_by_delim(&line, ','));
        uint64_t y = sv_to_u64(line);
        grid[y][x] = true;
        line = sv_chop_by_delim(&input, '\n');
    } while(line.count > 1);
    line = sv_chop_by_delim(&input, '\n'); sv_chop_left(&line, 11);
    char c = line.data[0]; sv_chop_left(&line, 2);
    uint64_t v = sv_to_u64(line);
    if(c == 'x') fold_x(grid, v);
    else if(c == 'y') fold_y(grid, v);
    
    size_t count = 0;
    for(size_t y=0; y<MATRIX_SIZE; y++){
        for(size_t x=0; x<MATRIX_SIZE; x++){
            count += grid[y][x] == true;
        }
    }
    return count;
}

void part_two() {
    String_View input = sv_slurp_file("input.txt");
    String_View line = sv_chop_by_delim(&input, '\n');
    bool grid[MATRIX_SIZE][MATRIX_SIZE] = {0};
    do {
        uint64_t x = sv_to_u64(sv_chop_by_delim(&line, ','));
        uint64_t y = sv_to_u64(line);
        grid[y][x] = true;
        line = sv_chop_by_delim(&input, '\n');
    } while(line.count > 1);
    while(input.count > 1) {
        line = sv_chop_by_delim(&input, '\n'); sv_chop_left(&line, 11);
        char c = line.data[0]; sv_chop_left(&line, 2);
        uint64_t v = sv_to_u64(line);
        if(c == 'x') fold_x(grid, v);
        else if(c == 'y') fold_y(grid, v);
    }
    
    size_t max_x = 0;
    size_t max_y = 0;
    size_t count = 0;
    for(size_t y=0; y<MATRIX_SIZE; y++)
        for(size_t x=0; x<MATRIX_SIZE; x++)
            if(grid[y][x]){
                max_x = x > max_x ? x : max_x;
                max_y = y > max_y ? x : max_y;
            }
    for(size_t y=0; y<=max_y; y++){
        for(size_t x=0; x<=max_x; x++)
            printf("%c", grid[y][x] ? '#' : '.');
        printf("\n");
    }
}

int main() {
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n");
    part_two();
}
