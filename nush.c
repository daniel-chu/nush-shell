#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "tokens.h"
#include "built_in.h"
#include "pipe.h"
#include "redirects.h"
#include "nush.h"
#include "other_operators.h"

int
execute(svec* tokens)
{
    if(tokens->size == 0) {
        return 1;
    }

    // ========== SEMICOLON ===========
    int semicolon_index = get_index_semicolon(tokens);
    if(semicolon_index != -1) {
        return nush_semicolon(semicolon_index, tokens);
    }

    // ========== REDIRECT ===========  
    int redirect_operator_index = get_redirect_operator_index(tokens);
    if(redirect_operator_index != -1) {
        return exec_redirect_cmd(redirect_operator_index, tokens);
    }

    // TODO ========== PIPE ===========  
    int pipe_index = get_pipe_index(tokens);
    if(pipe_index != -1) {
        return handle_and_exec_pipe(pipe_index, tokens);
    }

    // ========== OTHER OPERATORS ===========
    int other_operator_index = get_other_operator_index(tokens);
    if(other_operator_index != -1) {
        return exec_other_operator_func(other_operator_index, tokens);
    }

    // ========== BUILT INS ===========
    int built_in_cmd_code = get_built_in_cmd_code(tokens->data[0]);
    if(built_in_cmd_code != -1) {
        return exec_built_in_cmd(built_in_cmd_code, tokens);
    }

    // ========== PROGRAMS ===========
    int cpid;
    // if cpid != 0, we are in the parent process
    if((cpid = fork())) {

        // printf("Parent pid: %d\n", getpid());
        // printf("Parent knows child pid: %d\n", cpid);

        int status;
        waitpid(cpid, &status, 0);
        if (WIFEXITED(status)) {
            if(status != 0) {
                // printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
                return status;
            }
        }

        // printf("== executed program complete ==\n");

        // printf("child returned with wait code %d\n", status);
        if (WIFEXITED(status)) {
            // printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
        }

    } else {        // if cpid is 0 (the else case), we are in the child process
        // printf("Child pid: %d\n", getpid());
        // printf("Child knows parent pid: %d\n", getppid());

        char* cmd = tokens->data[0];
        char** args = tokens->data;

        // printf("****************** DEBUG START ********************************\n");
        // int ii;
        // for(ii = 0; ii < tokens->size; ++ii) {
        //     printf("TOKEN: %s\n", tokens->data[ii]);
        // }

        // for(ii = 0; ii < 3; ++ii) {
        //     printf("ARG: %s\n", args[ii]);
        // }
        // printf("****************** END DEBUG **************************\n");

        // printf("== executed program's output: ==\n");
        execvp(cmd, args);
        // printf("Can't get here, exec only returns on error.");
        exit(errno);
    }

    return 0;
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

    return 0;
}
