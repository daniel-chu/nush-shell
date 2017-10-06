#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "tokens.h"
#include "built_in.h"
#include "redirects.h"

// creates a null terminated array of arguments using the start (inclusive) and end index (exclusive)
// char**
// create_args(int start, int end, svec* tokens)
// {
//     int size = end - start + 2; // extra room for the null terminator
//     char** args = malloc(size * sizeof(char*));

//     int ii;
//     for(ii = 0; ii < tokens->size; ++ii) {
//         args[ii] = tokens->data[ii];
//     }
//     args[ii] = 0;

//     return args;
// }   

void
execute(svec* tokens)
{
    // TODO ========== REDIRECT ===========  
    int redirect_operator_index = get_redirect_operator_index(tokens);
    if(redirect_operator_index != -1) {
        exec_redirect_cmd(redirect_operator_index, tokens);
        return;
    }

    // TODO ========== PIPE ===========  

    // TODO ========== OTHER OPERATORS ===========


    // ========== BUILT INS ===========
    int built_in_cmd_code = get_built_in_cmd_code(tokens->data[0]);
    if(built_in_cmd_code != -1) {
        exec_built_in_cmd(built_in_cmd_code, tokens);
        return;
    }

    // ========== PROGRAMS ===========
    int cpid;
    // if cpid != 0, we are in the parent process
    if((cpid = fork())) {
        int status;
        waitpid(cpid, &status, 0);
        if (WIFEXITED(status)) {
            if(status != 0) {
                printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
            }
        }

    } else {        // if cpid is 0 (the else case), we are in the child process
        char* cmd = tokens->data[0];

        // char** args = create_args(0, tokens->size, tokens);

        char** args = tokens->data;

        // TODO DEBUG SECTION
        // printf("****************** DEBUG INFO ********************************\n");
        // int ii;
        // for(ii = 0; ii < tokens->size; ++ii) {
        //     printf("TOKEN: %s\n", tokens->data[ii]);
        // }

        // for(ii = 0; ii < 3; ++ii) {
        //     printf("ARG: %s\n", args[ii]);
        // }
        // printf("**************************************************************\n");
        execvp(cmd, args);
        // using errno.h, we can get the error code so it prints (rather than just giving us 0)
        exit(errno);
    }
}

int
main(int argc, char* argv[])
{
    if (argc == 1) {
        char cmd[256];
        for(;;) {
            printf("nush$ ");
            fflush(stdout);

            char* lc = fgets(cmd, 256, stdin);
            if(lc == 0) {
                break;
            }
            svec* tokens = tokenize_line(cmd);

            execute(tokens);
            free_svec(tokens);
        }
    } else {
        // TODO READ FROM FILE
        FILE* file = fopen(argv[1], "r");
        char cmd[256];
        for(;;) {
            char* lc = fgets(cmd, 256, file);
            if(lc == 0) {
                break;
            }
            svec* tokens = tokenize_line(cmd);
            execute(tokens);
            free_svec(tokens);    
        }
        fclose(file);
    }

    // if (argc == 1) {
    //     printf("nush$ ");
    //     fflush(stdout);
    //     fgets(cmd, 256, stdin);
    // }
    // else {
    //     memcpy(cmd, "echo", 5);
    // }

    // execute(cmd);


    return 0;
}
