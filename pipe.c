#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "nush.h"
#include "tokens.h"

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
    svec* left = svec_subvec(tokens, 0, index);
    svec* right = svec_subvec(tokens, index + 1, tokens->size);

    int pid;
    int pipes[2];
    int rv = pipe(pipes);
    if(rv == -1) {
        exit(1);
    }

    int cpid1;
    int cpid2;
    int status;

    if(!(cpid1 = fork())) {
        close(pipes[0]);
        // sets pipes[1] to be in file descriptor table as 1
        dup2(pipes[1], 1);

        execute(left);
        exit(0);
    }

    if(!(cpid2 = fork())) {
        close(pipes[1]);
        // sets pipes[0] to be in file descriptor table as 0
        dup2(pipes[0], 0);

        execute(right);
        exit(0);
    }

    close(pipes[0]);
    close(pipes[1]);

    waitpid(cpid1, &status, 0);
    waitpid(cpid2, &status, 0);

    return 0;
}
