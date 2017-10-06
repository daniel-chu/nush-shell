#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "tokens.h"
#include "built_in.h"

// creates a null terminated array of arguments using the start (inclusive) and end index (exclusive)
char**
create_args(int start, int end, svec* tokens)
{
    int size = end - start + 2; // extra room for the null terminator
    char** args = malloc(size * sizeof(char*));

    int ii;
    for(ii = 0; ii < tokens->size; ++ii) {
        args[ii] = tokens->data[ii];
    }
    args[ii] = 0;

    return args;
}   

void
execute(svec* tokens)
{
    // built in cmds
    int built_in_cmd_index = get_built_in_cmd_index(tokens->data[0]);
    if(built_in_cmd_index != -1) {
        exec_built_in_cmd(built_in_cmd_index, tokens);
    }

    // programs
    int cpid;
    // if cpid != 0, we are in the parent process
    if((cpid = fork())) {

        printf("Parent pid: %d\n", getpid());
        printf("Parent knows child pid: %d\n", cpid);

        int status;
        waitpid(cpid, &status, 0);

        printf("== executed program complete ==\n");

        printf("child returned with wait code %d\n", status);
        if (WIFEXITED(status)) {
            printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
        }

    } else {        // if cpid is 0 (the else case), we are in the child process
        printf("Child pid: %d\n", getpid());
        printf("Child knows parent pid: %d\n", getppid());

        char* cmd = tokens->data[0];

        char** args = create_args(0, tokens->size, tokens);

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

        printf("== executed program's output: ==\n");
        execvp(cmd, args);
        printf("Can't get here, exec only returns on error.\n");
        // using errno.h, we can get the error code so it prints (rather than just giving us 0)
        exit(errno);
    }
}

int
main(int argc, char* argv[])
{
    svec* tokens;
    char cmd[256];
    if (argc == 1) {
        for(;;) {
            printf("nush$ ");
            fflush(stdout);

            char* lc = fgets(cmd, 256, stdin);
            if(lc == 0) {
                break;
            }
            tokens = tokenize_line(cmd);

            execute(tokens);

            // TODO DEBUG SECTION
            // ***** PRINT TOKENS ******
            // int ii;
            // for(ii = 0; ii < tokens->size; ++ii) {
            //     printf("%s\n", tokens->data[ii]);
            // }
            // *************************

            free_svec(tokens);
        }
    } else {
        // TODO READ FROM FILE
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
