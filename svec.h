#ifndef SVEC
#define SVEC

typedef struct svec {
    char** data;
    int size;
    int capacity;
} svec;

svec* make_svec();
void free_svec(svec* xs);

void svec_push(svec* xs, char* x);

#endif