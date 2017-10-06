#ifndef BUILT_IN
#define BUILT_IN

#include "svec.h"

// returns length of the built in cmd array
int
lengthof_built_ins();

// gets the index of the function that handles the given built in cmd.
// if it is not a built in cmd, then return -1
int
get_built_in_cmd_index(char* cmd);

// runs the function that handles the built in cmd at index i in the array of pointers to functions
int
exec_built_in_cmd(int index, svec* tokens);

// does cd
int
nush_cd(svec* tokens);

// does exit
int
nush_exit(svec* tokens);


#endif