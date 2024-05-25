#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>


int main(){
	int pid, status=0, i=0,j=0;
	char **args= NULL;
	while(WEXITSTATUS(status)!=2){
		if ((pid=fork())==0){
			// Intalizations step
			char *p = getenv("PATH"), *_token = NULL, *command_token = NULL,*flags_token = NULL, _input[10000],cmd_path[10000]; 
			char* _path= (char*)calloc(strlen(p),sizeof(char)); 
			strcpy(_path,p);
			int wrdCount=0;
			
			printf("#-:"); //Prompt to command.
			fgets(_input,sizeof(_input),stdin);
			
			/*Counting the amount of words in the input string - allocating an array of strings according to the result.*/
			for (int k=1; k<strlen(_input); k++){
				if(isspace(_input[k])   &&   !isspace(_input[k-1]) )
					wrdCount++;			
			}
			_input[strcspn(_input,"\n")]= 0; // Avoiding newline tokens.
			
			args= (char**)calloc(wrdCount, sizeof(char**));
			
			
			/*-------------------------------------------------------*/
			command_token= strtok(_input," \t"); //Grabbing the name of the command- first field.
			if(strcmp(command_token,"leave")==0){
				exit(2); // User hard exit.
			}
			args[i++]=command_token; //Setting first string in the array to the name of command and advandcing the indexer.
			
			flags_token = strtok(NULL," \t");
			if (flags_token!=NULL){
				while (flags_token){
					args[i++]=flags_token; //Setting flag field, advanding indexer...
					flags_token = strtok(NULL," \t");	
				}
				flags_token=command_token;
			}
			
			
			args[i]=NULL; //Capping arguements sent to execv with NULL.
			
			
			_token=strtok(_path,":");
			while(_token){
				//Concatenating the name of the command to the directory token we're currently on.
				cmd_path[0] = 0;
				strcpy(cmd_path,_token);
				strcat(cmd_path,"/\0");
				strcat(cmd_path,command_token);
				
				if(flags_token!=NULL)
					execv(cmd_path,args);
					
				else
					execl(cmd_path,command_token,NULL); //ONE LINER COMMAND
				_token = strtok(NULL,":");
				
			}
			free(_path);
			free(args);
			
			exit(1);
			
		}
		
		else{
			pid=wait(&status); 
			if(WEXITSTATUS(status)==1){
			printf("File or command were not found.\n");
			}
		}
	
	}
	
	
	
	return 0;

}
