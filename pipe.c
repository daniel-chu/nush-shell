#include <stdio.h>
#include <string.h>
#include "nush.h"

int
get_pipe_index(svec* tokens)
{
    int ii;
    for (ii = 0; ii < tokens->size; ++ii) {
        if(strcmp(tokens->data[ii], "|") == 0) {
            return ii;
        }
    }
    return -1;
}

int
handle_and_exec_pipe(int index, svec* tokens)
{
    //TODO
    printf("PIPE AT INDEX: %d\n", index);
    return 0;
}
