#ifndef REDIRECTS
#define REDIRECTS

#include "svec.h"

// returns the index in the svec that the first operator is at
// (left associative so this is the one that you split on)
int get_redirect_operator_index(svec* tokens);

// executes the redirect command based on the redirect command at index in tokens
int exec_redirect_cmd(int index, svec* tokens);

// function that handles the redirect out command at index in tokens
int nush_redirect_out(int index, svec* tokens);

// function that handles the redirect in command at index in tokens
int nush_redirect_in(int index, svec* tokens);

#endif