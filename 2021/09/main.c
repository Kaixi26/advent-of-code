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

#define GET(input, ncols, x, y) (input.data[(y)*(ncols+1)+(x)] - '0')

bool check_low(String_View input, size_t ncols, size_t nlins, size_t x, size_t y){
    // check left
    if(x >= 1 && GET(input,ncols,x,y) >= GET(input,ncols,x-1,y))
        return false;
    // check right
    if(x < ncols-1 && GET(input,ncols,x,y) >= GET(input,ncols,x+1,y))
        return false;
    //check up
    if(y >= 1 && GET(input,ncols,x,y) >= GET(input,ncols,x,y-1))
        return false;
    if(y < nlins-1 && GET(input,ncols,x,y) >= GET(input,ncols,x,y+1))
        return false;

    return true;
}

size_t part_one() {
    String_View input = sv_slurp_file("input.txt");
    size_t ncols = 0;
    size_t nlins = 0;
    {
        String_View input_ = input;
        ncols = sv_chop_by_delim(&input_, '\n').count;
        for(nlins=0; nlins*(ncols+1)+2 < input.count; nlins++)
            ;
    }
    size_t sum = 0;
    for(size_t y=0; y<nlins; y++){
        for(size_t x=0; x<ncols; x++){
            if(check_low(input, ncols, nlins, x, y))
                sum += GET(input,ncols,x,y) + 1;
        }
    }
    return sum;
}

void ins(size_t vals[4], size_t val){
    size_t i;
    for(i=0; i<3 && vals[i] > val; i++)
        ;
    for(size_t j=3; i<j; j--)
        vals[j] = vals[j-1];
    vals[i] = val;
}

size_t calc_basin_aux(String_View input, size_t ncols, size_t nlins, size_t x, size_t y, bool* checked){
    if(checked[y*(nlins+1)+x] || GET(input,ncols,x,y) == 9) return 0;
    checked[y*(nlins+1)+x] = true;
    size_t count = 1;
    if(x >= 1 && GET(input,ncols,x,y) < GET(input,ncols,x-1,y))
        count += calc_basin_aux(input,ncols,nlins,x-1,y,checked);
    if(x < ncols-1 && GET(input,ncols,x,y) < GET(input,ncols,x+1,y))
        count += calc_basin_aux(input,ncols,nlins,x+1,y,checked);
    if(y >= 1 && GET(input,ncols,x,y) < GET(input,ncols,x,y-1))
        count += calc_basin_aux(input,ncols,nlins,x,y-1,checked);
    if(y < nlins-1 && GET(input,ncols,x,y) < GET(input,ncols,x,y+1))
        count += calc_basin_aux(input,ncols,nlins,x,y+1,checked);
    return count;
}

size_t calc_basin(String_View input, size_t ncols, size_t nlins, size_t x, size_t y){
    bool checked[ncols*(nlins+1)];
    memset(checked, false, ncols*(nlins+1)*sizeof(bool));
    return calc_basin_aux(input, ncols, nlins, x, y, checked);
}

size_t part_two() {
    String_View input = sv_slurp_file("input.txt");
    size_t ncols = 0;
    size_t nlins = 0;
    {
        String_View input_ = input;
        ncols = sv_chop_by_delim(&input_, '\n').count;
        for(nlins=0; nlins*(ncols+1)+2 < input.count; nlins++)
            ;
    }
    size_t max3[4] = {0};

    for(size_t y=0; y<nlins; y++){
        for(size_t x=0; x<ncols; x++){
            if(check_low(input, ncols, nlins, x, y))
                ins(max3, calc_basin(input,ncols,nlins,x,y));
        }
    }
    return max3[0]*max3[1]*max3[2];
}

int main() {
    printf("Part one: %ld\n", part_one());
    printf("Part two: %ld\n", part_two());
}
