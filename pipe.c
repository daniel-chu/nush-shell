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
exec_left_side_pipe(int pipes[], svec* tokens) {
    int cpid;
    if((cpid = fork())) {
    } else {
        close(pipes[0]);
        // close stdout
        close(1);
        // sets pipes[1] to be in file descriptor table as 1
        dup(pipes[1]);

        execute(tokens);
        exit(0);
        // execvp(tokens->data[0], tokens->data);
    }
    return 0;
}

int
exec_right_side_pipe(int pipes[], svec* tokens) {
    int cpid;
    if((cpid = fork())) {
    } else {
        close(pipes[1]);
        // close stdin
        close(0);
        // sets pipes[0] to be in file descriptor table as 0
        dup(pipes[0]);

        execute(tokens);
        exit(0);
        // execvp(tokens->data[0], tokens->data);
    }
    return 0;
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

    exec_left_side_pipe(pipes, left);
    exec_right_side_pipe(pipes, right);

    // TODO how to get it to delay printing nush$ immediately

    close(pipes[0]);
    close(pipes[1]);

    return 0;
}
