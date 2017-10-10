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

// removes the first and last characters, used to remove parens.
char*
remove_first_and_last_chars(char* string)
{
    int string_len = strlen(string);
    int result_len = string_len - 2;
    string++;

    char* result = malloc(result_len + 1);
    memcpy(result, string, result_len);
    result[result_len] = 0;

    return result;
}

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

    // ========= PARANTHESES ===========
    // if there is one token, and it starts and ends with a parentheses, we need to tokenize the
    // contents of the parentheses after removing the parens and then pass those tokens back into execute
    char* first_token = tokens->data[0];
    int starts_with_parens = strncmp(first_token, "(", 1) == 0;
    if(tokens->size == 1 && starts_with_parens) {
        char* inner_string = remove_first_and_last_chars(first_token);
        svec* inner_tokens = tokenize_line(inner_string);
        free(inner_string);

        int return_code = execute(inner_tokens);
        free(inner_tokens);
        return return_code;
    }

    // ========== REDIRECT ===========  
    int redirect_operator_index = get_redirect_operator_index(tokens);
    if(redirect_operator_index != -1) {
        return exec_redirect_cmd(redirect_operator_index, tokens);
    }

    // ========== PIPE ===========  
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
    if((cpid = fork())) {
        // parent
        int status;
        waitpid(cpid, &status, 0);

        if (WIFEXITED(status)) {
            if(status != 0) {
                return status;
            }
        }

    } else {
        // child
        char* cmd = tokens->data[0];
        char** args = tokens->data;

        execvp(cmd, args);
        exit(errno);
    }

    return 0;
}

int
main(int argc, char* argv[])
{
    if (argc == 1) {
        char cmd[256];
        char temp[256];
        for(;;) {
            printf("nush$ ");
            fflush(stdout);

            char* lc = fgets(temp, 256, stdin);
            if(lc == 0) {
                break;
            }

            strcpy(cmd, temp);

            // handles the \ character, allowing the user to continue entering on the next line
            while(temp[strlen(temp) - 2] == '\\') {
                printf("> ");

                // if the last two chars of command are \ and the null terminator,
                // we need to remove that before we continue appending
                if(cmd[strlen(cmd) - 2] == '\\') {
                    char new_cmd[256];
                    memcpy(new_cmd, cmd, strlen(cmd) - 2);
                    new_cmd[strlen(cmd) - 2] = 0;

                    strcpy(cmd, new_cmd);
                }

                lc = fgets(temp, 256, stdin);
                if(lc == 0) {
                    break;
                }                
                strncat(cmd, temp, 256 - strlen(cmd));
            }

            svec* tokens = tokenize_line(cmd);
            execute(tokens);
            free_svec(tokens);
        }
    } else {
        FILE* file = fopen(argv[1], "r");
        char cmd[256];
        char temp[256];
        for(;;) {
            char* lc = fgets(temp, 256, file);
            if(lc == 0) {
                break;
            }

            strcpy(cmd, temp);

            // handles the \ character, allowing the user to continue entering on the next line
            while(temp[strlen(temp) - 2] == '\\') {
                // if the last two chars of command are \ and the null terminator,
                // we need to remove that before we continue appending
                if(cmd[strlen(cmd) - 2] == '\\') {
                    char new_cmd[256];
                    memcpy(new_cmd, cmd, strlen(cmd) - 2);
                    new_cmd[strlen(cmd) - 2] = 0;

                    strcpy(cmd, new_cmd);
                }

                lc = fgets(temp, 256, file);
                if(lc == 0) {
                    break;
                }                
                strncat(cmd, temp, 256 - strlen(cmd));
            }

            svec* tokens = tokenize_line(cmd);
            execute(tokens);
            free_svec(tokens);
        }
        fclose(file);
    }

    return 0;
}
