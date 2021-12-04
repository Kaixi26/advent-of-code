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

typedef struct {
    uint64_t table[5][5];
    bool marked[5][5];
} bingo;

void debug(bingo* b){
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++)
            printf("%c%ld\t", b->marked[i][j] ? 'x' : ' ', b->table[i][j]);
        printf("\n");
    }
    printf("\n");
}

uint64_t bingo_readn(String_View input, bingo* bs){
    uint64_t bi;
    for(bi=0; input.count > 1; bi++){
        (void) sv_chop_by_delim(&input, '\n');
        for(int i=0; i<5; i++){
            String_View row = sv_chop_by_delim(&input, '\n');
            for (int j=0; j<5; j++){
                row = sv_trim(row);
                String_View col = sv_chop_by_delim(&row, ' ');
                bs[bi].table[i][j] = sv_to_u64(col);
            }
        }
    }
    return bi;
}

bool bingo_check(bingo* b, uint64_t loc[2]){
    bool rh = true;
    for(int i=0; i<5; i++)
        rh = rh && b->marked[i][loc[1]];

    bool rv = true;
    for(int j=0; j<5; j++)
        rv = rv && b->marked[loc[0]][j];
    return rh || rv;
}

bool bingo_mark(bingo* b, uint64_t v, uint64_t loc[2]){
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            if(b->table[i][j] == v){
                b->marked[i][j] = true;
                loc[0] = i;
                loc[1] = j;
                return true;
            }
    return false;
}


size_t part_one(){
    String_View input = sv_slurp_file("input.txt");
    String_View numbers = sv_chop_by_delim(&input, '\n');
    bingo bingos[124] = {0};
    uint64_t bingos_count = bingo_readn(input, bingos);

    size_t foundi = 0;
    uint64_t foundn = 0;
    bool found = false;
    while(numbers.count > 1 && !found){
        uint64_t num = sv_to_u64(sv_chop_by_delim(&numbers, ','));
        for(size_t i=0; i<bingos_count; i++){
            uint64_t loc[2];
            if(bingo_mark(&bingos[i], num, loc)){
                if(bingo_check(&bingos[i], loc)){
                    foundi = i;
                    foundn = num;
                    found = true;
                    break;
                }
            }
        }
    }


    size_t board_sum = 0;
    for(size_t i=0; i<5; i++)
        for(size_t j=0; j<5; j++)
            if(!bingos[foundi].marked[i][j])
                board_sum += bingos[foundi].table[i][j];
    return board_sum*foundn;
}

size_t part_two(){
    String_View input = sv_slurp_file("input.txt");
    String_View numbers = sv_chop_by_delim(&input, '\n');
    bingo bingos[124] = {0};
    bool won[124] = {0};
    uint64_t bingos_count = bingo_readn(input, bingos);

    size_t foundi = 0;
    uint64_t foundn = 0;
    bool found = false;
    size_t won_count = 0;
    while(numbers.count > 1 && !found){
        uint64_t num = sv_to_u64(sv_chop_by_delim(&numbers, ','));
        for(size_t i=0; i<bingos_count; i++){
            uint64_t loc[2];
            if(!won[i] && bingo_mark(&bingos[i], num, loc)){
                if(bingo_check(&bingos[i], loc)){
                    won[i] = true;
                    won_count++;
                    if(won_count == bingos_count){
                        foundi = i;
                        foundn = num;
                        found = true;
                        break;
                    }
                }
            }
        }
    }


    size_t board_sum = 0;
    for(size_t i=0; i<5; i++)
        for(size_t j=0; j<5; j++)
            if(!bingos[foundi].marked[i][j])
                board_sum += bingos[foundi].table[i][j];
    return board_sum*foundn;
}

int main(){
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
