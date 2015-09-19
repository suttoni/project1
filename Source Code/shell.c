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
#include <fcntl.h>
#include <stdbool.h>

/*Function declarations*/
/*Basic setup function*/
void my_setup(){


}
/*Prompt for the user*/
void my_prompt(){
    char host[128];
    char currentWorkingDirectory[1024];
    
    gethostname(host, sizeof host);
    getcwd(currentWorkingDirectory, sizeof currentWorkingDirectory);
    
    printf("%s@%s:%s $", getlogin(), host, currentWorkingDirectory);

}
/*Executes external commands like ls, echo*/
void my_execute(char **cmd){		// for instance char* cmd[4]={"/bin/ls", "-l", "-a", NULL}
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
}

void Exit(char **cmd)	// For built-in command "exit". Just call it then quit shell
{
	if ( strcmp( cmd[0], "exit") == 0 || strcmp( cmd[0], "Exit") == 0)
	{
		printf("Exiting shell...\n\n(shell terminates)\n");
		exit(0);
	}
}

void Echo(char **cmd)	// For built-in "echo". could add if(cmd[0] =="echo") if needed.
{
	int i = 1;
	
	while( cmd[i] != NULL )
	{
		if( cmd[i][0] != '$' )	//if not requiring an EnvVar, just echo it.
			printf("%s ", cmd[i]);
		else				// if an EnvVar
		{
			char newargu[ strlen(cmd[i])];	// char array to remove '$'
			char* p1 = &(cmd[i][1]);	
			char* p2 = newargu;
			while( *p1 != '\0' ){	// copy except '$'
				*p2 = *p1;
				p1++;	p2++;
			}
			*p2 = *p1;	//copy NULL to newargu end.
			
			
			char* env;
			if( env = getenv( newargu ) )	// EnvVar is there
				printf("%s ", env);
			else					// requiring undefined variable
				printf("(%s: Undefined variable) ", newargu);
		}
		i++;
	}
	printf("\n");
}

bool is_iored(char** cmd) // check if right format for iored
{						// call by output_red and input_red
	if( strcmp(cmd[0],">") == 0 || strcmp(cmd[0],"<") == 0) // if begin with <>, fail.
		return false;
	else 
	{
		int i = 0;
		while( cmd[i] != NULL ) i++;	// for cmd XXX XXX > NULL case, fail
		if( strcmp(cmd[i-1], ">")==0 || strcmp(cmd[i-1], "<")==0 )
			return false;
		else 
			return true;
	}
}


void output_red(char** cmd)	//set up fd for output redirection
{
	if( is_iored(cmd))
	{
		int k = 0;
		while( cmd[k] != NULL ) k++; // k->cmd.end(). find end of cmd.
		char filename[strlen(cmd[k-1])];
		if( strcpy( filename, cmd[k-1]) == NULL)
		{
			printf("strcpy filename failed!\n");
			exit(1);
		}
		
		char path[256];
		strcpy( path, getenv("PWD"));
		strcat(path, "/");
		strcat(path, filename);		// make char[] path 
		
		int fd = open( path, O_RDWR|O_CREAT|O_TRUNC);
		
		pid_t fpid = fork();
		if (fpid < 0)
		{
			perror("io fork failed!\n");
			exit(1);
		}
		if( fpid == 0 ) // child
		{
			close(STDOUT_FILENO);
			dup(fd);
			close(fd);
			/*
			
				execute other commands here
			
			*/
		//	Echo(cmd); // my example run
			_exit(0); 	//better not use exit(). buffer issue. look it up.
			
		}
		else{
			close(fd);
			waitpid(fpid, NULL, 0);
		}
	}
	else
		printf("Undefined cmd format!\n"); //if wrong io format
	
}

void input_red(char** cmd)		// for input redirection
{
	if( is_iored(cmd))
	{
		int k = 0;
		while( cmd[k] != NULL ) k++; // k->cmd.end()
		char filename[strlen(cmd[k-1])];
		if( strcpy( filename, cmd[k-1]) == NULL)
		{
			printf("strcpy filename failed!\n");
			exit(1);
		}
		
		char path[256];
		strcpy( path, getenv("PWD"));
		strcat(path, "/");
		strcat(path, filename);		//making path/filename
		
		char command[10];
		if( strcpy(command, cmd[0]) )
		{
			printf("strcpy command failed!\n");
			exit(1);
		}							//making command(echo, ls,,,)
		
		int fd = open( path, O_RDONLY);
		if(fd == 0) { printf("open file failed!\n"); return;} 
								// check availability of file
		pid_t fpid = fork();
		if (fpid < 0)
		{
			perror("io fork failed!\n");
			exit(1);
		}
		if( fpid == 0 ) // child
		{
			close(STDIN_FILENO);
			dup(fd);
			close(fd);
			
			// t_line = my_read() from file!
			// t_cmd = my_parse() from t_line I guess.
			
			// check, judge and execute t_cmd(argu from file).
			
			_exit(0);
		}
		else{			// parent
			close(fd);
			waitpid(fpid, NULL, 0);
		}
		
		
	}
	else
		printf("Undefined cmd format!\n");
}

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
