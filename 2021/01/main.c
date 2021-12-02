#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

#define SV_IMPLEMENTATION
#include "../sv.h"

size_t part_one(){
    String_View input = sv_slurp_file("input.txt");
    size_t count = 0;
    uint64_t prev = sv_to_u64(sv_chop_by_delim(&input, '\n'));
    while(input.count > 1){
        uint64_t curr = sv_to_u64(sv_chop_by_delim(&input, '\n'));
        if(curr > prev)
            count++;
        prev = curr;
    }
    return count;
}

size_t part_two(){
    String_View input = sv_slurp_file("input.txt");
    size_t count = 0;
    uint64_t values[4];
    size_t next_value = 0;
    for(int i=0; i<3; i++)
        values[next_value++] = sv_to_u64(sv_chop_by_delim(&input, '\n'));

    while(input.count > 1){
        values[(next_value++)%4] = sv_to_u64(sv_chop_by_delim(&input, '\n'));
        uint64_t prev_sum = values[(next_value-4)%4] + values[(next_value-3)%4] + values[(next_value-2)%4];
        uint64_t curr_sum = values[(next_value-3)%4] + values[(next_value-2)%4] + values[(next_value-1)%4];
        if(curr_sum > prev_sum)
            count++;
    }
    return count;
}

int main(){
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
