#ifndef PIPE
#define PIPE

#include "svec.h"

int get_pipe_index(svec* tokens);

int handle_and_exec_pipe(int index, svec* tokens);

#endif