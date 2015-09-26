/* This file contains the declarations of the functions
 * that are to be used in the Shell program. 
 * ----------------------------------------------------
 * Team: Satisfries SP3
 * Authors: Ian Sutton, Sai Gunasegaran, Yilin Wang
 * FSUID: iss13, sg13z, yw12c 
 * Language: C
 * Completed using: Ubuntu Linux 3.16.0-49-generic 
 */

#ifndef SHELL_UTIL_H
#define SHELL_UTIL_H

#include <stdbool.h>

void my_prompt();
void my_execute(char **cmd);
void my_execute(char **cmd);
void Echo(char **cmd);
void output_red(char** cmd);
void input_red(char** cmd);
void my_clean();

char *my_read();
char **my_parse(char *line);

bool is_iored(char** cmd);

int cd(char **args);
#endif