#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

//Prototypes
int runProgram(char*, int);
int killProgram(int);

int main()
{
	//Input Buffer
	char command[1024];
	char parameter[1024];
	int engine = 1, success = 0;
	
	//Prompt
	while(engine == 1)
	{
		printf("# ");
		scanf("%s",command);
		scanf("%[^\n]",parameter);
		
		
		//Command - Run
		if(strcmp("run", command) == 0)
		{
			success = runProgram(parameter, 0);
			if(success == 247)
				printf("The program has failed to run!\n");
		}
		//Command - Quit
		if(strcmp("quit", command) == 0)
		{
			engine = 0;
		}
		
		//Command - Background
		if(strcmp("background", command) == 0)
		{
			runProgram(parameter, 1);
		}
		
		//Command - Murder
		if(strcmp("murder", command) == 0)
		{
			success = killProgram(atoi(parameter));
		}
	}
	
	//Clear Parameter
	parameter = NULL;
	
	return 0;	
}

int runProgram(char* parameter, int mode)
{
	//Variable for Child Status Check
	int childStatus;
	int childCrash;
	
	//Fork
	int childID = fork();
	
	//Parameter Break-Up
	int i, paraCounter = 0;
	char * parameters[16];
	
	parameters[0] = malloc(sizeof(char) * 1024);
	char * buffer = strtok(parameter, " ");
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
	
	//Parameter Count
	paraCounter = i - 2;
	
	//Child Transformation
	if(childID == 0)
	{
		childCrash = execv(parameters[0],&parameters[1]);
		//Execution Fail Check
		if(childCrash == -1)
			exit(247);
	}
	
	//Parent Check for Child Termination
	if(mode == 0)
	{
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
	else
		printf("The child's process ID is: %d\n",childID);
	
	return 0;
}

int killProgram(int pid)
{
	return kill(pid, SIGKILL);
}


