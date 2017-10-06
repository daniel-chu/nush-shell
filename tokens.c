#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "svec.h"

int
isoperator(char cc)
{
    char operators[] = {'<', '>', '|', '&', ';'};

    int operators_size = sizeof(operators) / sizeof(operators[0]);

    int ii;
    for(ii = 0; ii < operators_size; ++ii) {
        if(cc == operators[ii]) {
            return 1;
        }
    }

    return 0;
}

char*
read_operator(char* line)
{
    char* lookaheadCounter = line;
    int sizeString = 0;
    while(*lookaheadCounter != 0 && isoperator(*lookaheadCounter)) {
        ++sizeString;
        ++lookaheadCounter;
    }

    char* operator = malloc(sizeString + 1);
    memcpy(operator, line, sizeString);
    operator[sizeString] = 0;

    return operator;
}

char*
read_string(char* line)
{
    char* lookaheadCounter = line;
    int sizeString = 0;
    while(*lookaheadCounter != 0 && !isspace(*lookaheadCounter) && !isoperator(*lookaheadCounter)) {
        ++sizeString;
        ++lookaheadCounter;
    }

    char* argOrCmd = malloc(sizeString + 1);
    memcpy(argOrCmd, line, sizeString);
    argOrCmd[sizeString] = 0;

    return argOrCmd;
}

void
handle_line(char* line, svec* xs)
{
    while(*line != 0) {
        if(isspace(*line)) {
            line++;
            continue;
        }

        if(isoperator(*line)) {
            char* operator = read_operator(line);
            svec_push(xs, operator);
            line += strlen(operator);
            continue;
        }

        // read next arg or cmd
        char* argOrCmd = read_string(line);

        // increment pointer by size of that arg/cmd
        int lengthArgCmd = strlen(argOrCmd);
        line += lengthArgCmd;

        // adds the arg/cmd into our svec of tokens
        svec_push(xs, argOrCmd);
    }

}

svec*
tokenize_line(char* line)
{
    svec* xs;
    xs = make_svec();
    handle_line(line, xs);

    return xs;
}