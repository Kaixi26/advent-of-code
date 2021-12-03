#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>

#define SV_IMPLEMENTATION
#include "../sv.h"

#define VEC_IMPLEMENTATION
#define VEC_TYPE String_View
#define VEC_SUFF(V) V
#include "../vec.h"

size_t part_one(){
    String_View input = sv_slurp_file("input.txt");
    size_t size;
    {
        String_View _input = input;
        size = sv_chop_by_delim(&_input, '\n').count;
    }
    uint64_t counts[16][3] = {0};
    while(input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        for(size_t i=0; i<size; i++)
            counts[i][(line.data[i]-'0')%2]++;
    }
    for(size_t i=0; i<size; i++)
        counts[i][2] = counts[i][0] > counts[i][1] ? 0 : 1;
    uint64_t gamma = 0;
    uint64_t epsilon = 0;
    for(size_t i=0; i<size; i++){
        gamma += counts[i][2] << (size-i-1);
        epsilon |= (1 ^ counts[i][2]) << (size-i-1);
    }
    return gamma*epsilon;
}

size_t part_two(){
    String_View input = sv_slurp_file("input.txt");
    size_t size;
    {
        String_View _input = input;
        size = sv_chop_by_delim(&_input, '\n').count;
    }

    Vec lines_oxy[2] = {0};
    Vec lines_scr[2] = {0};
    unsigned char active;

    while(input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        vec_push(&lines_oxy[0], line);
        vec_push(&lines_scr[0], line);
    }

    active = 0;
    for(size_t i=0; lines_oxy[active].top > 1 && i<size; i++){
        uint64_t counts[2] = {0};
        for(size_t li=0; li<lines_oxy[active].top; li++){
            String_View line = lines_oxy[active].data[li];
            counts[line.data[i]-'0']++;
        }
        active = (1-active);
        unsigned char expect = counts[0] > counts[1] ? '0' : '1';
        while(lines_oxy[1-active].top > 0){
            String_View line = vec_pop_unchecked(&lines_oxy[1-active]);
            if(line.data[i] != expect)
                continue;
            vec_push(&lines_oxy[active], line);
        }
    }
    String_View rating_oxy = vec_pop_unchecked(&lines_oxy[active]);

    active = 0;
    for(size_t i=0; lines_scr[active].top > 1 && i<size; i++){
        uint64_t counts[2] = {0};
        for(size_t li=0; li<lines_scr[active].top; li++){
            String_View line = lines_scr[active].data[li];
            counts[line.data[i]-'0']++;
        }
        active = (1-active);
        unsigned char expect = counts[0] <= counts[1] ? '0' : '1';
        while(lines_scr[1-active].top > 0){
            String_View line = vec_pop_unchecked(&lines_scr[1-active]);
            if(line.data[i] != expect)
                continue;
            vec_push(&lines_scr[active], line);
        }
    }
    String_View rating_scr = vec_pop_unchecked(&lines_scr[active]);

    uint64_t oxy = 0;
    uint64_t scr = 0;
    for(size_t i=0; i<size; i++){
        oxy |= (rating_oxy.data[i] - '0') << (size-i-1);
        scr |= (rating_scr.data[i] - '0') << (size-i-1);
    }
    return oxy*scr;
}

int main(){
    //printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
