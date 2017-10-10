#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "built_in.h"
#include "svec.h"

char* built_in_cmd_strings[] = {"cd", "exit"};

int (*built_in_cmds[]) (svec* tokens) = {
    *nush_cd,
    *nush_exit
};

int
lengthof_built_ins() {
    return sizeof(built_in_cmd_strings) / sizeof(built_in_cmd_strings[0]);
}

int
get_built_in_cmd_code(char* cmd) {
    int ii;
    for(ii = 0; ii < lengthof_built_ins(); ++ii) {
        if(strcmp(cmd, built_in_cmd_strings[ii]) == 0) {
            return ii;
        }
    }
    return -1;
}

int
exec_built_in_cmd(int code, svec* tokens) {
    return built_in_cmds[code](tokens);
}

int
nush_cd(svec* tokens) {
    if(tokens->data[1] == 0 || strcmp(tokens->data[1], "~") == 0) {
        // change to home directory here if required
    } else {
        chdir(tokens->data[1]);
    }
    return 0;
}

int
nush_exit(svec* tokens) {
    exit(0);
    return 0;
}