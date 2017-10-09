#ifndef OTHER_OPERATORS
#define OTHER_OPERATORS

#include "svec.h"

// returns length of the other_operators array
int lengthof_other_operators();

// returns the code of the operator that str is (the index used to pull associated function out of array of functions)
int get_other_operator_code(char* cmd);

// returns the index in the svec that the operator is at
int get_other_operator_index(svec* tokens);

int exec_other_operator_func(int index, svec* tokens);

int nush_and(int index, svec* tokens);

int nush_or(int index, svec* tokens);

int nush_background(int index, svec* tokens);

#endif