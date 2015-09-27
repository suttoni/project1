/* A shell implementation in the C language.
 * The following functions are function definitions from
 * shell_util.h. 
 * -----------------------------------------------------
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

//Define delimiter macro for parse function
#define DELIM " \t\r\n\a"

/* Prompt for the user */
void my_prompt(){
    char host[128];
    char curr_working_directory[1024];
    
    gethostname(host, sizeof host);
    getcwd(curr_working_directory, sizeof curr_working_directory);
    
    printf("%s@%s:%s $", getlogin(), host, curr_working_directory);
}


/* Reads data from standard input */
char* my_read(){
	/* Begin by creating a buffer variable for user input
	   with a arbitrary block size of 1024 */
	char *line;
	size_t bufsize = 0; // have getline allocate a buffer for us
	getline(&line, &bufsize, stdin);
	
	return line;
}

/* Parses the line provided by my_read */
char **my_parse(char *line){
	/* Begin by creating a buffer variable for user input
	   with a arbitrary block size of 1024 */
	int buffer_size = 64;
	int i = 0;	

	/* Dyanmically allocated buffer for user input */
	char  *token;
	char **token_storage = (char**)calloc(buffer_size, sizeof(char*));
	
	/* Standard error checking */
	if(!token_storage){
		fprintf(stderr, "Allocation Error in my_parse function\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, DELIM);

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

		}

		token = strtok(NULL, DELIM);
	}
	token_storage[i] = NULL;
	return token_storage;
}

/* Executes external commands like ls, echo */
/* for instance char* cmd[4]={"/bin/ls", "-l", "-a", NULL} */
void my_execute(char **cmd){		
	pid_t pid;
	char newString[256];
	char binString[] = "/bin/";

	strcat(newString, binString);
	strcat(newString, cmd[0]);

	pid = fork();
	if(pid == 0){
		if( execv(newString, cmd) < 0){
      		perror("execv error ");
      		exit(1);
    		}
	}

	else
		waitpid(pid, NULL, 0);
}

/* For built-in command "exit". Just call it then quit shell */
void Exit(char **cmd){
	if ( strcmp( cmd[0], "exit") == 0 || strcmp( cmd[0], "Exit") == 0)
	{
		printf("Exiting shell...\n\n(shell terminates)\n");
		exit(0);
	}
}

/* For built-in "echo". could add if(cmd[0] =="echo") if needed */
void Echo(char **cmd){
	int i = 1;
	
	while( cmd[i] != NULL ){
		if( cmd[i][0] != '$' )								// if not requiring an EnvVar, just echo it.
			printf("%s ", cmd[i]);
		else{												// if an EnvVar	
			char new_argu[ strlen(cmd[i])];					// char array to remove '$'
			char* p1 = &(cmd[i][1]);	
			char* p2 = new_argu;

			while( *p1 != '\0' ){							// copy except '$'
				*p2 = *p1;
				p1++;	p2++;
			}
			*p2 = *p1;										//copy NULL to new_argu end.
			
			
			char* env;
			if( env = getenv(new_argu) )					// EnvVar is there
				printf("%s ", env);	
			else											// requiring undefined variable
				printf("(%s: Undefined variable) ", new_argu);
		}
		i++;
	}
	printf("\n");
}

/* For built-in CD command */
int cd(char **cmd){
	/* Error checking to make sure there is a path to change directory */
	int ret;

	/* If no argument is given, cd to home */
	if (cmd[1] == NULL){
		ret = chdir(getenv("HOME"));
	}

	else{
		ret = chdir(cmd[1]);
	}

	return ret;
}

/* Check if right format for iored */
bool is_iored(char** cmd){	
/* Call by output_red and input_red */
	if( strcmp(cmd[0],">") == 0 || strcmp(cmd[0],"<") == 0) // if begin with <>, fail.
		return false;

	else{
		int i = 0;
		while( cmd[i] != NULL ) i++;						// for cmd XXX XXX > NULL case, fail
		if( strcmp(cmd[i-1], ">") == 0 || strcmp(cmd[i-1], "<") == 0 )
			return false;
		else 
			return true;
	}
}

/* Set up fd for output redirection */
void output_red(char** cmd){
	if( is_iored(cmd)){
		int k = 0;
		while( cmd[k] != NULL ) k++; 						// k->cmd.end(). find end of cmd.
		char filename[strlen(cmd[k-1])];

		if( strcpy( filename, cmd[k-1]) == NULL){
			printf("strcpy filename failed!\n");
			exit(1);
		}
		
		char path[256];
		strcpy( path, getenv("PWD") );
		strcat( path, "/" );
		strcat( path, filename );							// make char[] path 
		
		int fd = open( path, O_RDWR|O_CREAT|O_TRUNC);
		
		pid_t fpid = fork();
		if (fpid < 0){
			perror("io fork failed!\n");
			exit(1);
		}

		if( fpid == 0 ){ 									// child
			close(STDOUT_FILENO);
			dup(fd);
			close(fd);
			check_command(cmd);	
			_exit(0); 	//better not use exit(). buffer issue. look it up.
			
		}

		else{
			close(fd);
			waitpid(fpid, NULL, 0);
		}
	}

	else
		printf("Undefined cmd format!\n"); 					//if wrong io format
}

/* For input redirection */
void input_red(char** cmd){
	if( is_iored(cmd))
	{
		int k = 0;
		while( cmd[k] != NULL ) k++; 						// k->cmd.end()
		char filename[strlen(cmd[k-1])];
		if( strcpy( filename, cmd[k-1]) == NULL){
			printf("strcpy filename failed!\n");
			exit(1);
		}
		
		char path[256];
		strcpy( path, getenv("PWD") );
		strcat( path, "/" );
		strcat( path, filename );							//making path/filename
		
		char command[10];
		if( strcpy(command, cmd[0]) ){
			printf("strcpy command failed!\n");
			exit(1);
		}													//making command(echo, ls,,,)
		
		int fd = open( path, O_RDONLY);
		if(fd == 0) { printf("open file failed!\n"); return;} 
															// check availability of file
		pid_t fpid = fork();
		if (fpid < 0){
			perror("io fork failed!\n");
			exit(1);
		}

		if( fpid == 0 ){ 									// child
			close(STDIN_FILENO);
			dup(fd);
			close(fd);
			
			// t_line = my_read() from file!
			// t_cmd = my_parse() from t_line I guess.
			
			// check, judge and execute t_cmd(argu from file).
			
			_exit(0);
		}

		else{	 											// parent
			close(fd);
			waitpid(fpid, NULL, 0);
		}				
	}

	else
		printf("Undefined cmd format!\n");
}

void check_command(char **cmd){

	if (strcmp(cmd[0], "echo") == 0)
		Echo(cmd);
	else if (strcmp(cmd[0], "exit") == 0)
		Exit(cmd);
	else 
		my_execute(cmd);
}

/* Cleans up dynamically allocated resources */
void my_clean(char *line, char **cmd){
	free(line);
	free(cmd);
}
