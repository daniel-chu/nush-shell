#ifndef OTHER_OPERATORS
#define OTHER_OPERATORS

#include "svec.h"

// returns the index in the svec that the operator is at
int get_other_operator_index(svec* tokens);

// returns the index of the first semicolon, -1 if none.
int get_index_semicolon(svec* tokens);

// runs the correct function based on the operator at the given index on the given list of tokens
int exec_other_operator_func(int index, svec* tokens);

// handles and
int nush_and(int index, svec* tokens);

// handles or
int nush_or(int index, svec* tokens);

// handles background processes
int nush_background(int index, svec* tokens);

// handles semicolons
int nush_semicolon(int index, svec* tokens);

#endif