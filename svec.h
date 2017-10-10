#ifndef SVEC
#define SVEC

typedef struct svec {
    char** data;
    int size;
    int capacity;
} svec;

// makes an empty svec with capacity 2 and size 0
svec* make_svec();

// frees svec
void free_svec(svec* xs);

// pushes a char* onto the end of the given svec
void svec_push(svec* xs, char* x);

// returns a sub svec consisting of elements from the given xs from index start (inclusive) to end (exclusive)
svec* svec_subvec(svec* xs, int start, int end);

#endif