#ifndef BUILT_IN
#define BUILT_IN

#include "svec.h"

// returns the code of the built in cmd that cmd is
// (the index used to pull associated function out of array of functions)
int
get_built_in_cmd_code(char* cmd);

// runs the function that handles the built in cmd at index i in the array of pointers to functions
int
exec_built_in_cmd(int code, svec* tokens);

// does cd
int
nush_cd(svec* tokens);

// does exit
int
nush_exit(svec* tokens);

#endif