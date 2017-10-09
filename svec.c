#include "svec.h"

#include <stdlib.h>
#include <string.h>

svec*
make_svec() {
    svec* xs = malloc(sizeof(svec));
    xs->size = 0;
    xs->capacity = 2;
    xs->data = malloc((xs->capacity + 1) * sizeof(char*));

    return xs;
}

void
free_svec(svec* xs) {
    int ii;
    for(ii = 0; ii < xs->size; ++ii) {
        if(xs->data[ii] != 0) {
            free(xs->data[ii]);
        }
    }
    free(xs->data);
    free(xs);
}

void
svec_push(svec* xs, char* x) {
    if(xs->size >= xs->capacity) {
        xs->capacity *= 2;
        xs->data = (char**) realloc(xs->data, (xs->capacity + 1) * sizeof(char*));
    }

    // note: when I need to put into index i, I will need to free(xs->data[ii]) before i insert (or mem leak)
    // this is currently fine because we are only pushing to the end of the vector here
    xs->data[xs->size++] = strdup(x);
}

svec*
svec_subvec(svec* xs, int start, int end) {
    svec* result = make_svec();

    int ii;
    for(ii = start; ii < end; ++ii) {
        svec_push(result, xs->data[ii]);
    }
    result->data[result->size] = 0;

    return result;
}