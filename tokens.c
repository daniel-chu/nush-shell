#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "svec.h"

char*
read_quotes_token(char* line)
{    
    char* lookaheadCounter = line + 1;
    int sizeString = 0;

    // passes the first quote

    while(*lookaheadCounter != 0 && *lookaheadCounter != '\"') {
        ++sizeString;
        ++lookaheadCounter;
    }

    char* result = malloc(sizeString + 1);
    memcpy(result, line + 1, sizeString);
    result[sizeString] = 0;

    return result;
}

char*
read_paren_token(char* line)
{
    int paren_counter = 0;
    
    char* lookaheadCounter = line;
    int sizeString = 0;

    do {
        if(*lookaheadCounter == '(') {
            paren_counter++;
        } else if(*lookaheadCounter == ')') {
            paren_counter--;
        }
        ++sizeString;
        ++lookaheadCounter;
    } while (*lookaheadCounter != 0 && paren_counter > 0);

    char* result = malloc(sizeString + 1);
    memcpy(result, line, sizeString);
    result[sizeString] = 0;

    return result;
}

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
    char* argOrCmd;
    char* operator;
    char* paren_token;
    char* quote_token;

    while(*line != 0) {
        if(isspace(*line)) {
            line++;
        } else if(*line == '(') {
            paren_token = read_paren_token(line);
            svec_push(xs, paren_token);
            line += strlen(paren_token);
            free(paren_token);
        } else if(*line == '\"') {
            quote_token = read_quotes_token(line);
            svec_push(xs, quote_token);
            // + 2 because we want to skip the open/close quotes
            line += strlen(quote_token) + 2;
            free(quote_token);
        } else if(isoperator(*line)) {
            operator = read_operator(line);
            svec_push(xs, operator);
            line += strlen(operator);
            free(operator);
        } else {
            // read next arg or cmd
            argOrCmd = read_string(line);

            // increment pointer by size of that arg/cmd
            int lengthArgCmd = strlen(argOrCmd);
            line += lengthArgCmd;

            // adds the arg/cmd into our svec of tokens
            svec_push(xs, argOrCmd);
            free(argOrCmd); 
        }


    }

}

svec*
tokenize_line(char* line)
{
    svec* xs;
    xs = make_svec();
    handle_line(line, xs);
    xs->data[xs->size] = 0;
    return xs;
}