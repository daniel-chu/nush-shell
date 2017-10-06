#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "redirects.h"
#include "svec.h"

char* redirect_operators[] = {">", "<"};

int (*redirect_functions[]) (int index, svec* tokens) = {
    *nush_redirect_out,
    *nush_redirect_in
};

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
lengthof_redirect_operators() {
    return sizeof(redirect_operators) / sizeof(redirect_operators[0]);
}

int
nush_redirect_out(int index, svec* tokens) {
    printf("REDIRECT OUT AT INDEX: %d\n", index);
    return 0;
}

int
nush_redirect_in(int index, svec* tokens) {
    printf("REDIRECT IN AT INDEX: %d\n", index);
    return 0;
}

int
exec_redirect_cmd(int index, svec* tokens) {
    int redirect_operator_code = get_redirect_operator_code(tokens->data[index]);
    return redirect_functions[redirect_operator_code](index, tokens);
}