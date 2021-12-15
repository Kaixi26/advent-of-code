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
    uint16_t x;
    uint16_t y;
    size_t cost;
} Candidate;

#define SIZE 500
typedef struct {
    uint8_t board[SIZE][SIZE];
    uint64_t cost[SIZE][SIZE];
    bool checked[SIZE][SIZE];
    size_t width;
    size_t height;
} State;

Candidate* cs;
size_t cs_count = 0;

void dump_state(State* st){
    for(size_t y=0; y<st->height; y++){
        for(size_t x=0; x<st->width; x++)
            printf("%d", st->checked[y][x] ? 0 : st->board[y][x]);
        printf("\n");
    }
    printf("\n");
}

void swap(Candidate* c1, Candidate* c2){
    Candidate tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}

void insert(State* st, Candidate c){
    if(st->checked[c.y][c.x]) return;
    if(c.x >= st->width) return;
    if(c.y >= st->height) return;

    for(size_t i=0; i<cs_count; i++)
        if(c.x == cs[i].x && c.y == cs[i].y){
            cs[i].cost = cs[i].cost < c.cost ? cs[i].cost : c.cost;
            for(; i>0 && cs[i].cost < cs[i+1].cost; i++)
                swap(&cs[i], &cs[i+1]);
            return;
        }

    cs[cs_count++] = c;
    for(size_t i=cs_count-1; i>0 && cs[i].cost > cs[i-1].cost; i--)
        swap(&cs[i], &cs[i-1]);
}

void step(State* st){
    Candidate c = cs[--cs_count];

    st->checked[c.y][c.x] = true;
    st->cost[c.y][c.x] = c.cost;
    insert(st, (Candidate){.x = c.x+1, .y = c.y, .cost = c.cost + st->board[c.y][c.x+1]});
    insert(st, (Candidate){.x = c.x, .y = c.y+1, .cost = c.cost + st->board[c.y+1][c.x]});
}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    cs = calloc(100*100, sizeof(Candidate));
    State st = {0};
    {
        String_View _input = input;
        st.width = sv_chop_by_delim(&_input, '\n').count;
    }
    while(input.count > 1) {
        String_View line = sv_chop_by_delim(&input, '\n');
        for(size_t i=0; i<line.count; i++)
            st.board[st.height][i] =  line.data[i]-'0';
        st.height++;
    }
    insert(&st, (Candidate) {0});
    while(!st.checked[st.height-1][st.width-1])
        step(&st);
    free(cs);
    return st.cost[st.height-1][st.width-1];
}

size_t part_two() {
    String_View input = sv_slurp_file("example.txt");
    cs = calloc(SIZE*SIZE, sizeof(Candidate));
    State st = {0};
    {
        String_View _input = input;
        st.width = sv_chop_by_delim(&_input, '\n').count;
    }
    while(input.count > 1) {
        String_View line = sv_chop_by_delim(&input, '\n');
        for(size_t i=0; i<line.count; i++)
            st.board[st.height][i] =  line.data[i]-'0';
        st.height++;
    }
    for(size_t y=0; y<st.height; y++)
        for(size_t x=0; x<st.width; x++)
            for(size_t i=0; i<5; i++)
                for(size_t j=0; j<5; j++)
                    st.board[y+i*st.height][x+j*st.width] = 
                        ((st.board[y][x]+i+j-1) % 9) + 1;
    st.width *= 5;
    st.height *= 5;

    insert(&st, (Candidate) {0});
    //dump_state(&st);
    while(!st.checked[st.height-1][st.width-1])
        step(&st);
    return st.cost[st.height-1][st.width-1];
}


int main() {
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
