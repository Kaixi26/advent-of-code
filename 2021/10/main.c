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

#define VEC_IMPLEMENTATION
#define VEC_TYPE size_t
#define VEC_SUFF(V) V
#include "../vec.h"

bool open_brkt(char x){
    return x=='(' || x=='[' || x=='{' || x=='<';
}

bool close_brkt(char x){
    return x==')' || x==']' || x=='}' || x=='>';
}

bool match_brkt(char begin, char end){
    return (begin == '(' && end == ')')
        || (begin == '[' && end == ']')
        || (begin == '{' && end == '}')
        || (begin == '<' && end == '>');
}

char matching_brkt(char c){
    switch(c){
        case '(': return ')';
        case '[': return ']';
        case '{': return '}';
        case '<': return '>';
        default: assert(0 && "Unreachable");
    }
}

size_t brkt_points(String_View* line){
    size_t points[256] = {0};
    points[')'] = 3;
    points[']'] = 57;
    points['}'] = 1197;
    points['>'] = 25137;
    char begin = sv_chop_left(line, 1).data[0];
    while(open_brkt(line->data[0])){
        size_t points = brkt_points(line);
        if(points)
            return points;
    }
    char end = sv_chop_left(line, 1).data[0];
    if(!match_brkt(begin,end))
        return points[(size_t)end];
    return 0;
}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    size_t count = 0;
    while(input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        count += brkt_points(&line);
    }
    return count;
}

size_t brkt_points2(String_View* line){
    size_t points[256] = {0};
    points[')'] = 1;
    points[']'] = 2;
    points['}'] = 3;
    points['>'] = 4;
    size_t pts = 0;
    char begin = sv_chop_left(line, 1).data[0];
    while(open_brkt(line->data[0])){
        pts += brkt_points2(line);
    }
    char end = sv_chop_left(line, 1).data[0];
    pts *= 5;
    if(end == '\n'){
        pts += points[(size_t)matching_brkt(begin)];
    }
    return pts;
}

int cmp_size_t(const void* x, const void* y){
    if (*((size_t*)x) > *((size_t*)y)) return  1;
    else if (*((size_t*)x) < *((size_t*)y)) return -1;
    else return 0;
}

size_t part_two() {
    String_View input = sv_slurp_file("input.txt");
    Vec points = {0};
    while(input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        { // skip complete ones
            String_View line_ = line;
            if(brkt_points(&line_) > 0)
                continue;
        }
        size_t pts = 0;
        while(line.count > 1)
            pts = brkt_points2(&line);
        vec_push(&points, pts);
    }
    qsort(points.data, points.top, sizeof(size_t), cmp_size_t);
    return points.data[points.top/2];
}

int main() {
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
