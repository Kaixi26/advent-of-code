#ifndef VEC_H_
#define VEC_H_

#include <stdlib.h>

#if !defined(VEC_TYPE) || !defined(VEC_SUFF)
#error "Must define VEC_TYPE and VEC_T for type and struct name."
#endif

#define VEC_T VEC_SUFF(Vec)
#define VEC_INIT_SIZE 4
#define VEC_REALLOC_SCALE 2

typedef struct {
    VEC_TYPE* data;
    size_t top;
    size_t size;
} VEC_SUFF(Vec);

bool VEC_SUFF(vec_push)(VEC_T* v, VEC_TYPE x){
    if(v->top >= v->size){
        size_t new_size = v->size < VEC_INIT_SIZE ? VEC_INIT_SIZE : v->size*VEC_REALLOC_SCALE;
        v->data = realloc(v->data, new_size*sizeof(VEC_TYPE));
        if(!v->data)
            return false;
        v->size = new_size;
    }
    v->data[v->top++] = x;
    return true;
}

VEC_TYPE VEC_SUFF(vec_pop_unchecked)(VEC_T* v){
    return v->data[--v->top];
}


void VEC_SUFF(vec_debug)(VEC_T* v, void (*debug)(VEC_TYPE x)){
    printf("{ data: [");
    for(size_t i=0; i<v->top; i++){
        debug(v->data[i]);
        printf(",");
    }
    printf("], top: %ld, size: %ld }\n", v->top, v->size);
}

void VEC_SUFF(vec_free)(VEC_T* v){
    free(v->data);
    *v = (VEC_T){0};
}

#ifdef SV_IMPLEMENTATION

#endif
#endif
