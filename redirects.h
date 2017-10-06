#ifndef REDIRECTS
#define REDIRECTS

#include "svec.h"

int get_redirect_operator_code(char* str);

int get_redirect_operator_index(svec* tokens);

int lengthof_redirect_operators();

int get_redirect_operator_index(svec* tokens);

int exec_redirect_cmd(int index, svec* tokens);

int nush_redirect_out(int index, svec* tokens);

int nush_redirect_in(int index, svec* tokens);

#endif