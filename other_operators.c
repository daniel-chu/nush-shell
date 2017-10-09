#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "nush.h"
#include "other_operators.h"
#include "svec.h"

char* other_operators[] = {"&&", "||", "&"};

int (*other_operator_funcs[]) (int index, svec* tokens) = {
    *nush_and,
    *nush_or,
    *nush_background
};

int
lengthof_other_operators() {
    return sizeof(other_operators) / sizeof(other_operators[0]);
}

int
get_other_operator_code(char* cmd) {
    int ii;
    for(ii = 0; ii < lengthof_other_operators(); ++ii) {
        if(strcmp(cmd, other_operators[ii]) == 0) {
            return ii;
        }
    }
    return -1;
}

int
get_other_operator_index(svec* tokens) {
    int ii;
    for (ii = 0; ii < tokens->size; ++ii) {
        if(get_other_operator_code(tokens->data[ii]) != -1) {
            return ii;
        }
    }
    return -1;
}

int
get_index_semicolon(svec* tokens) {
    int ii;
    for (ii = 0; ii < tokens->size; ++ii) {
        if(strcmp(tokens->data[ii], ";") == 0) {
            return ii;
        }
    }
    return -1;
}

int
exec_other_operator_func(int index, svec* tokens) {
    int code = get_other_operator_code(tokens->data[index]);
    return other_operator_funcs[code](index, tokens);
}

int
nush_and(int index, svec* tokens) {
    svec* left = svec_subvec(tokens, 0, index);
    svec* right = svec_subvec(tokens, index + 1, tokens->size);

    if(execute(left) == 0) {
        execute(right);
    }

    return 0;
}

int
nush_or(int index, svec* tokens) {
    svec* left = svec_subvec(tokens, 0, index);
    svec* right = svec_subvec(tokens, index + 1, tokens->size);

    if(execute(left) != 0) {
        execute(right);
    }

    return 0;
}

int
nush_background(int index, svec* tokens) {
    // TODO check over this
    svec* command = svec_subvec(tokens, 0, index);

    int cpid;
    if((cpid = fork())) {
        int status;
    } else {
        // execute(command);
        execvp(command->data[0], command->data);
    }

    free_svec(command);
    return 0;
}

int
nush_semicolon(int index, svec* tokens) {
    svec* left = svec_subvec(tokens, 0, index);
    svec* right = svec_subvec(tokens, index + 1, tokens->size);

    execute(left);
    return execute(right);
}