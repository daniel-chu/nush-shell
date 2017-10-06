#ifndef TOKENS
#define TOKENS

#include "svec.h"

// tells us if cc is an operator
int isoperator(char cc);

// returns the operator at the line pointer
char* read_operator(char* line);

// returns the string at the line pointer
char* read_string(char* line);

// goes through the passed in line and adds the tokens from it into xs
void handle_line(char* line, svec* xs);

// tokenizes a line into an svec
svec* tokenize_line(char* line);

#endif