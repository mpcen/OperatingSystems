#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//Prototypes
int runProgram(char**, int, int);
int killProgram(int);

int main()
{
	//Input Buffer
	char command[1024];
	char raw[1024];
	char* parameters[32], *buffer;
	int i, engine = 1, success = 0;
	
	//Prompt
	while(engine == 1)
	{
		//Main Prompt
		printf("# ");
		scanf("%[^\n]%*c", raw);
		
		//String Tokenizer
		parameters[0] = malloc(sizeof(char) *  1024);
		buffer = strtok(raw, " ");
		strcpy(parameters[0], buffer);
		
		for(i = 1; buffer != NULL; i++)
		{
			buffer = strtok(NULL, " ");
			if(buffer != NULL)
			{
				parameters[i] = malloc(sizeof(char) * 1024);
				strcpy(parameters[i], buffer);
			}
			else
				parameters[i] = NULL;
		}
		
		//Command - Run
		if(strcmp("run", parameters[0]) == 0)
		{
			success = runProgram(parameters, 0, 1);
			if(success == 247)
				printf("The program has failed to run!\n");
		}
		//Command - Quit
		else if(strcmp("quit", parameters[0]) == 0)
		{
			engine = 0;
		}
		
		//Command - Background
		else if(strcmp("background", parameters[0]) == 0)
		{
			runProgram(parameters, 1, atoi(parameters[1]));
		}
		
		//Command - Murder
		else if(strcmp("murder", parameters[0]) == 0)
		{
			success = killProgram(atoi(parameters[1]));
		}
		
		else if(strcmp("repeat", parameters[0]) == 0)
		{
			success = runProgram(parameters, 2, atoi(parameters[1]));
		}
		
		//Command - Invalid
		else
			printf("Command not understood!\n");
		
		//Free Memory
		i--;
		for(; i >= 0; i--)
			free(parameters[i]);
		
	}
	
	return 0;	
}

int runProgram(char** parameter, int mode, int n)
{
	//Variable for Child Status Check
	int childStatus;
	int childCrash;
	int childID;

	//Run Mode (Wait for Child)
	if(mode == 0)
	{
		//Fork & Wait
		childID = fork();
		
		//Child Transformation
		if(childID == 0)
		{
			childCrash = execv(parameter[1],&parameter[1]);
			//Execution Fail Check
			if(childCrash == -1)
				exit(247);
		}
		
		wait(&childStatus);
	
		//Exit and Report Status
		if(childStatus == 247)
		{
			printf("Program has failed to execute. Please check your syntax!\n");
			return 247;	
		}
		else
			return 0;
	}
	
	//Background Process
	else if(mode == 1)
	{
		//Fork & Wait
		childID = fork();	
		
		//Child Transformation
		if(childID == 0)
		{
			childCrash = execv(parameter[1],&parameter[1]);
			//Execution Fail Check
			if(childCrash == -1)
				exit(247);
		}
		
		//Print Child ID
		printf("The child's process ID is: %d\n",childID);			
	}
	
	//Repeated Background Processes
	else
	{
		printf("PIDs: ");
		for(;n > 0; n--)
		{
			//Child Fork & Transformation
			childID = fork();
			if(childID == 0)
			{
				childCrash = execv(parameter[2],&parameter[2]);
				//Execution Fail Check
				if(childCrash == -1)
					exit(247);
			}
			printf("%d ",childID);
		}
		printf("\n");
	}
	
	return 0;
}

int killProgram(int pid)
{
	return kill(pid, SIGKILL);
}
