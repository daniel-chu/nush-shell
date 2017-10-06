#ifndef TOKENS
#define TOKENS

#include "svec.h"

int isoperator(char cc);
char* read_operator(char* line);
char* read_string(char* line);
void handle_line(char* line, svec* xs);
svec* tokenize_line(char* line);

#endif