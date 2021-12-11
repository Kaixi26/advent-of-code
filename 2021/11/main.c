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

typedef struct {
    unsigned char energy[10][10];
} Grid;

void flash(Grid* grid, size_t x, size_t y){
    for(ssize_t i=-1; i<=1; i++)
        for(ssize_t j=-1; j<=1; j++){
            if(y+i <= 9 && x+j <= 9)
                grid->energy[y+i][x+j]++;
        }
}

size_t compute_flashes(Grid* start){
    bool flashed[10][10] = {0};
    size_t flashes = 0;
    Grid end = {0};
    memcpy(&end, start, sizeof(unsigned char)*100);
    do {
        memcpy(start, &end, sizeof(unsigned char)*100);
        for(size_t y=0; y<10; y++)
            for(size_t x=0; x<10; x++)
                if(!flashed[y][x] && end.energy[y][x] > 9){
                    flashed[y][x] = true;
                    flash(&end, x, y);
                    flashes++;
                }
    } while (memcmp(&end, start, sizeof(unsigned char)*100) != 0);
    
    for(size_t y=0; y<10; y++)
        for(size_t x=0; x<10; x++)
            if(end.energy[y][x] > 9)
                end.energy[y][x] = 0;
    memcpy(start, &end, sizeof(unsigned char)*100);
    return flashes;
}

size_t step(Grid* grid){
    for(size_t y=0; y<10; y++)
        for(size_t x=0; x<10; x++)
            grid->energy[y][x]++;
    return compute_flashes(grid);
}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    Grid init = {0};
    for(size_t y=0; y<10; y++)
        for(size_t x=0; x<10; x++)
            init.energy[y][x] = input.data[y*11+x] - '0';

    size_t flashes = 0;
    for(size_t i=0; i<100; i++)
        flashes += step(&init);

    return flashes;
}

size_t part_two() {
    String_View input = sv_slurp_file("input.txt");
    Grid init = {0};
    for(size_t y=0; y<10; y++)
        for(size_t x=0; x<10; x++)
            init.energy[y][x] = input.data[y*11+x] - '0';

    size_t steps = 0;
    size_t flashes;
    do {
         flashes = step(&init);
         steps++;
    } while(flashes != 100);

    return steps;
}

int main() {
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
