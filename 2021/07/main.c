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
#define VEC_TYPE size_t
#define VEC_SUFF(V) V
#include "../vec.h"

void debug(size_t x){
    printf("%ld", x);
}

size_t checkfor(Vec* v, size_t x){
    int count = 0;
    for (size_t i=0; i<v->top; i++)
        count += v->data[i] > x ? v->data[i] - x : x - v->data[i];
    return count;
}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    Vec v = {0};
    while (input.count > 1){
        uint64_t num = sv_to_u64(sv_chop_by_delim(&input, ','));
        vec_push(&v, num);
    }
    //vec_debug(&v, debug);
    size_t min = 0;
    size_t max = v.data[0];
    double avg = 0;
    for(size_t i=0; i<v.top; i++){
        min = min < v.data[i] ? min : v.data[i];
        max = max > v.data[i] ? max : v.data[i];
        avg += v.data[i];
    }
    avg /= v.top;
    //printf("%ld %ld %f\n", min, max, avg);
    size_t min2 = checkfor(&v, 0);
    for(size_t i=min; i<max; i++){
        size_t x = checkfor(&v, i);
        min2 = min2 < x ? min2 : x;
        //printf("%ld: %ld\n", i, x);
    }

    return min2;
}

size_t checkfor2(Vec* v, size_t x){
    int count = 0;
    for (size_t i=0; i<v->top; i++){
        size_t diff = v->data[i] > x ? v->data[i] - x : x - v->data[i];
        count += (diff*(diff+1))/2;
    }
    return count;
}

size_t part_two() {
    String_View input = sv_slurp_file("input.txt");
    Vec v = {0};
    while (input.count > 1){
        uint64_t num = sv_to_u64(sv_chop_by_delim(&input, ','));
        vec_push(&v, num);
    }
    //vec_debug(&v, debug);
    size_t min = 0;
    size_t max = v.data[0];
    double avg = 0;
    for(size_t i=0; i<v.top; i++){
        min = min < v.data[i] ? min : v.data[i];
        max = max > v.data[i] ? max : v.data[i];
        avg += v.data[i];
    }
    avg /= v.top;
    //printf("%ld %ld %f\n", min, max, avg);
    size_t min2 = checkfor2(&v, 0);
    for(size_t i=min; i<max; i++){
        size_t x = checkfor2(&v, i);
        min2 = min2 < x ? min2 : x;
        //printf("%ld: %ld\n", i, x);
    }

    return min2;
}

typedef struct {
    size_t days[9];
} Fish;

int main() {
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
