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

int main(){
	char *line;
	char **cmd;
	
	while(1){
		my_prompt();
		line = my_read();
		cmd = my_parse(line);
		
		int i;
		char *ifIR=NULL, *ifOR=NULL;
		for(i=0; cmd[i]!=NULL;i++)
		{
			ifOR = strchr(cmd[i], '>');
			if( ifOR!=NULL ) break;	// if cmd contains >
		}
		for(i=0; cmd[i]!=NULL;i++)
		{
			ifIR = strchr(cmd[i], '<');
			if( ifIR!=NULL ) break;// if cmd contains <
		}
	//////////////////////
		if(cmd[0] != NULL)
		{
			if( strcmp(cmd[0], "etime")==0 )
				Etime(cmd);
			else if( ifOR!=NULL )
				output_red(cmd);
			else if( ifIR!=NULL )
				input_red(cmd);
			else if((strcmp(cmd[0], "echo") == 0)||(strcmp(cmd[0], "exit") == 0))
				check_command(cmd);
			else
				my_execute(cmd);
		}
		
		my_clean(line, cmd);
	}

	return 0;
}
