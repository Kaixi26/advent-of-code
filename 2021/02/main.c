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


size_t part_one(){
    String_View input = sv_slurp_file("input.txt");
    int64_t pos = 0;
    int64_t depth = 0;
    while(input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        String_View direction = sv_chop_by_delim(&line, ' ');
        uint64_t amount = sv_to_u64(line);
        switch(direction.data[0]){
            case 'f':
                pos += amount;
                break;
            case 'd':
                depth += amount;
                break;
            case 'u':
                depth -= amount;
                break;
            default:
                assert(0 && "Unreachable!");
        }
    }
    return pos*depth;
}

size_t part_two(){
    String_View input = sv_slurp_file("input.txt");
    int64_t pos = 0;
    int64_t depth = 0;
    int64_t aim = 0;
    while(input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        String_View direction = sv_chop_by_delim(&line, ' ');
        uint64_t amount = sv_to_u64(line);
        switch(direction.data[0]){
            case 'f':
                pos += amount;
                depth += aim*amount;
                break;
            case 'd':
                aim += amount;
                break;
            case 'u':
                aim -= amount;
                break;
            default:
                assert(0 && "Unreachable!");
        }
    }
    return pos*depth;
}


int main(){
    printf("Part one: %ld\n", part_one());
    printf("Part one: %ld\n", part_two());
}
