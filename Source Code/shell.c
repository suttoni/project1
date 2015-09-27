/* A shell implementation in the C language.
 * The main shell driver program that utilizes the 
 * functions from shell_util.c in order to create a shell
 * ------------------------------------------------------
 * Team: Satisfries SP3
 * Authors: Ian Sutton, Sai Gunasegaran, Yilin Wang
 * FSUID: iss13, sg13z, yw12c 
 * Language: C
 * Completed using: Ubuntu Linux 3.16.0-49-generic  
 */

/*Included libraries*/
#include "shell_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>

int main(){
	char *line;
	char **cmd;
	
	while(1){
		//Setup
		//Print Prompt
		//Read Input
		//Transform input
		//Match against patterns
		//Execute command
		//Print results
		//Cleanup
		//my_setup();

		my_prompt();
		line = my_read();
		cmd = my_parse(line);
		check_command(cmd);
		my_clean(line, cmd);
	}

	return 0;
}
