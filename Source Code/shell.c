/* A shell implementation in the C language.

// Basic information
Team: Satisfries SP3
Authors: Ian Sutton, Sai Gunasegaran, Yilin Wang
FSUID: iss13 (Sai and Yilin, please enter your FSUIDs)
Language: C
Compiled: Ubuntu/gcc

// Acknowledgments/Cited resources
"Learning the Shell" 
- http://www.linuxcommand.org

"Implementation of a Job Control Shell" 
- http://www.gnu.org/software/libc/manual/html_node/

*/

/*Included libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

/*Function declarations*/
/*Basic setup function*/
void my_setup(){


}
/*Prompt for the user*/
void my_prompt(){


}
/*Executes external commands like ls, echo*/
void my_execute(char **cmd){		// for instance cmd[]={"/bin/ls", "-l", "-a", NULL}
	pid_t pid;
	if( pid = fork() == 0)
	{
		if( execv( cmd[0],cmd) < 0)
    	{
      		perror("execv error ");
      		exit(1);
    	}
	}
	else
		waitpid(pid, NULL, 0);
		
/*Cleans up dynamically allocated resources*/
void my_clean(){


}
/*Reads data from standard input*/
char *my_read(){
	/* Begin by creating a buffer variable for user input
	   with a arbitrary block size of 1024 */
	int buffer_size = 1024;

	/* Dyanmically allocated buffer for user input */
	char *buffer = (char*)calloc(buffer_size, sizeof(char));

	/* Standard error checking */
	if (!buffer){
		fprintf(stderr, "Allocation Error in my_read function\n");
		exit(EXIT_FAILURE);
	}

}

/*The following functions are used by my_parse function*/

char **parse_whitespace(char *line){

	//Take in line, insert into array
	//Remove leading whitespace
	//Remove trailing whitespace
	//Remove all but one whitespace between arguments
	//If no whitespace, add between arguments 
	//if there is a special character |, <, >, &, $, ~
	//use isspace
	
}

char **parse_arguments(char *line){


}

char **resolve_paths(char **args){

}

char **expand_variables(char **args){

}

/*Parses the line provided by my_read*/
char **my_parse(char *line){
	/* Begin by creating a buffer variable for user input
	   with a arbitrary block size of 1024 */
	int buffer_size = 64;
	int i = 0;		

	/* Dyanmically allocated buffer for user input */
	char  *delim = " ";
	char  *token;
	char **token_storage = (char**)calloc(buffer_size, sizeof(char*));
	

	/* Standard error checking */
	if(!token_storage){
		fprintf(stderr, "Allocation Error in my_parse function\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, delim);

	while(token != NULL){

		token_storage[i] = token;
		++i;

		/* If the ith position in token_storage is larger 
		   than the buffer_size, reallocate memory */
		if(i >= buffer_size){
			/* Double buffer_size */
			buffer_size += buffer_size;

			/* reallocate token_storage to support new size */
			token_storage = realloc(token_storage, sizeof(char*) * buffer_size);

			/* Again, check for allocation error */
			if(!token_storage){
				fprintf(stderr, "Allocation Error in my_parse function\n");
				exit(EXIT_FAILURE);
			}

			token = strtok(NULL, delim);
		}

		token_storage[i] = NULL;
		return token_storage;
	}


}

int main(){
	char *line;
	char **cmd;
	
	/*Read, Eval, Print loop
	while(1){
		//Setup
		//Print Prompt
		//Read Input
		//Transform input
		//Match against patterns
		//Execute command
		//Print results
		//Cleanup

		/*
		my_setup();
		my_prompt();
		line = my_read();
		cmd = my_parse();
		my_execute(cmd);
		my_clean();
	}*/

	return 0;
}
