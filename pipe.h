#ifndef PIPE
#define PIPE

#include "svec.h"

// gets the index that the pipe is at
int get_pipe_index(svec* tokens);

// handles the pipe at the given index on the given tokens
int handle_and_exec_pipe(int index, svec* tokens);

#endif