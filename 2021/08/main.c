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

typedef enum {
    TOP,
    TOPL,
    TOPR,
    MID,
    BOTL,
    BOTR,
    BOT,
    LIGHT_COUNT,
} Light;

typedef struct {
    bool lights[LIGHT_COUNT];
} Num;

bool num_eq(Num n1, Num n2){
    for(Light i=0; i<LIGHT_COUNT; i++)
        if(n1.lights[i] != n2.lights[i])
            return false;
    return true;
}

Light char_to_light(char c){
    return c - 'a';
}

Num calc_num(String_View num){
    Num n = {0};
    for(size_t i=0; i<num.count; i++)
        n.lights[char_to_light(num.data[i])] = true;
    return n;
}

void decode(Light assoc[LIGHT_COUNT], Num num){
    (void) num;
    (void) assoc;
}

void calc_line(String_View line, Light assoc[LIGHT_COUNT]){
    memset(assoc, -1, LIGHT_COUNT*sizeof(Light));
    Num nums[10] = {0};
    String_View strs[10] = {0};

    for(size_t i=0; i<10; i++){
        strs[i] = sv_chop_by_delim(&line, ' ');
        if(strs[i].count == 2) nums[1] = calc_num(strs[i]);
        if(strs[i].count == 3) nums[7] = calc_num(strs[i]);
        if(strs[i].count == 4) nums[4] = calc_num(strs[i]);
    }

    { // find value for TOP
        for(Light i=0; i<LIGHT_COUNT; i++)
            if(nums[1].lights[i] ^ nums[7].lights[i])
                assoc[TOP] = i;
    }

    { // find value for MID, TOPR, BOTR
        for(size_t i=0; i<10; i++){
            if(strs[i].count != 6) continue;
            Num num = calc_num(strs[i]);
            for(Light i=0; i<LIGHT_COUNT; i++)
                if(nums[1].lights[i] && ! num.lights[i]){ // must be 6
                    nums[6] = num;
                    assoc[TOPR] = i;
                    for(Light j=0; j<LIGHT_COUNT; j++)
                        if(nums[1].lights[j] && i != j)
                            assoc[BOTR] = j;
                } else if(nums[4].lights[i] && ! num.lights[i]){ // must be 0
                    nums[0] = num;
                    assoc[MID] = i;
                }
        }
    }

    { // find value for BOTL
        for(size_t i=0; i<10; i++){
            if(strs[i].count != 5) continue;
            Num num = calc_num(strs[i]);
            if(!num_eq(num, nums[0]) && !num_eq(num, nums[6])){ // must be 9
                nums[9] = num;
                for(Light j=0; j<LIGHT_COUNT; j++)
                    if(!num.lights[j])
                        assoc[BOTL] = j;
            }
        }
    }

    { // find value for BOT
        for(size_t i=0; i<10; i++){
            if(strs[i].count != 5) continue;
            Num num = calc_num(strs[i]);
            size_t diff = 0;
            for(Light i=0; i<LIGHT_COUNT; i++)
                diff += nums[7].lights[i] != num.lights[i];
            if(diff != 2) continue; // must be 3
            nums[3] = num;
            for(Light i=0; i<LIGHT_COUNT; i++)
                if(!(assoc[MID] == i) && (nums[7].lights[i] != num.lights[i]))
                    assoc[BOT] = i;
        }
    }

    { // find value for TOPL
        for(Light i=0; i<LIGHT_COUNT; i++){
            // try to find i
            bool found = false;
            for(int j=0; j<LIGHT_COUNT; j++)
                if(assoc[j] == i) found = true;
            if(!found){
                assoc[TOPL] = i;
                break;
            }
        }
    }


    for(size_t i=0; i<10; i++){
        printf("%ld: ", i);
        for(Light j=0; j<LIGHT_COUNT; j++)
            printf("%d", nums[i].lights[j]);
        printf("\n");
    }

    for(Light i=0; i<LIGHT_COUNT; i++)
        printf("%d <- %d\n", i, assoc[i]);

}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    size_t count = 0;
    while (input.count > 1){
        String_View line = sv_chop_by_delim(&input, '\n');
        (void)sv_chop_by_delim(&line, '|');
        line = sv_trim(line);
        while(line.count > 1){
            String_View n = sv_chop_by_delim(&line, ' ');
            switch(n.count){
                case 2:
                case 3:
                case 4:
                case 7:
                    count++;
                default:
                    break;
            }
        }
    }
    return count;
}

size_t part_two() {
    String_View input = sv_slurp_file("example.txt");
    while (input.count > 1){
        Light assoc[LIGHT_COUNT];
        String_View line = sv_chop_by_delim(&input, '\n');
        calc_line(line, assoc);
    }
    return 0;
}


int main() {
    printf("Part one: %ld\n", part_one());
    //printf("Part two: %ld\n", part_two());
}
