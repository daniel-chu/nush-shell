#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "redirects.h"
#include "svec.h"
#include "nush.h"

char* redirect_operators[] = {">", "<"};

int (*redirect_functions[]) (int index, svec* tokens) = {
    *nush_redirect_out,
    *nush_redirect_in
};

int
lengthof_redirect_operators() {
    return sizeof(redirect_operators) / sizeof(redirect_operators[0]);
}

int
get_redirect_operator_code(char* str) {
    int ii;
    for(ii = 0; ii < lengthof_redirect_operators(); ++ii) {
        if(strcmp(str, redirect_operators[ii]) == 0) {
            return ii;
        }
    }
    return -1;
}

int
get_redirect_operator_index(svec* tokens) {
    int ii;
    for (ii = 0; ii < tokens->size; ++ii) {
        if(get_redirect_operator_code(tokens->data[ii]) != -1) {
            return ii;
        }
    }
    return -1;
}

int
exec_redirect_cmd(int index, svec* tokens) {
    int redirect_operator_code = get_redirect_operator_code(tokens->data[index]);
    return redirect_functions[redirect_operator_code](index, tokens);
}

int
nush_redirect_out(int index, svec* tokens) {
    svec* left = svec_subvec(tokens, 0, index);
    svec* right = svec_subvec(tokens, index + 1, tokens->size);

    char* file_name = right->data[0];
    // gives us file pointer to write to
    FILE* write_to = fopen(file_name, "w");
    int write_fileno = fileno(write_to);

    int cpid;
    if((cpid = fork())) {
        int status;
        waitpid(cpid, &status, 0);
    } else {    // child
        // close stdout
        close(1);
        // set file to be 1 in file descriptor table
        char rv = dup(write_fileno);

        // TODO IF WE NEED TO RECURSE (EXECUTE) ON THE LEFT SIDE, FIGURE THIS OUT
        // execute(left);
        execvp(left->data[0], left->data);
    }

    free_svec(left);
    free_svec(right);

    close(write_fileno);
    return 0;
}

int
nush_redirect_in(int index, svec* tokens) {
    svec* left = svec_subvec(tokens, 0, index);
    svec* right = svec_subvec(tokens, index + 1, tokens->size);

    char* file_name = right->data[0];
    // gives us file pointer to read from
    FILE* read_file = fopen(file_name, "r");
    int read_from_fileno = fileno(read_file);

    int cpid;
    if((cpid = fork())) {
        int status;
        waitpid(cpid, &status, 0);
    } else {    // child
        // close stdin
        close(0);
        // set file to be 0 in file descriptor table
        char rv = dup(read_from_fileno);

        execvp(left->data[0], left->data);
    }

    free_svec(left);
    free_svec(right);

    close(read_from_fileno);
    return 0;
}