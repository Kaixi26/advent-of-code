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

#define VEC_IMPLEMENTATION
#define VEC_TYPE unsigned char
#define VEC_SUFF(V) V
#include "../vec.h"


void print(unsigned char i){
    printf("%d", i);
}

void skip_day(Vec* v){
    size_t ctop = v->top;
    for(size_t i=0; i<ctop; i++){
        if(v->data[i] == 0){
            v->data[i] = 6;
            vec_push(v, 8);
        } else {
            v->data[i]--;
        }
    }
}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    Vec v = {0};
    while (input.count > 1){
        uint64_t num = sv_to_u64(sv_chop_by_delim(&input, ','));
        vec_push(&v, num);
    }

    int days = 80;
    for (int i=0; i<days; i++)
        skip_day(&v);

    return v.top;
}

typedef struct {
    size_t days[9];
} Fish;

Fish skip_day_2(Fish old){
    Fish new = {0};
    for(int i=0; i<8; i++){
        new.days[i] = old.days[i+1];
    }

    new.days[8]  = old.days[0];
    new.days[6] += old.days[0];
    return new;
}

size_t part_two() {
    String_View input = sv_slurp_file("input.txt");
    Fish fish = {0};

    while (input.count > 1){
        uint64_t num = sv_to_u64(sv_chop_by_delim(&input, ','));
        fish.days[num]++;
    }

    int days = 300;
    for (int i=0; i<days; i++){
        fish = skip_day_2(fish);
    }

    size_t total = 0;
    for(int i=0; i<9; i++)
        total += fish.days[i];

    return total;
}


int main() {
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
