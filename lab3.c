#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define Lab3InputBuffLength 512
#define Lab3ArgumentsCount 16

#define Lab3ErrorForking 1
#define Lab3ErrorCWDTooLong 2

//#define Lab3Debug

#ifndef null
#define null NULL
#endif

void MakeArgs(char **Args,char * Line)
{
	char * Token = strtok(Line, " \n\r");
	int Counter = 0;

	while(Token) {
#ifdef Lab3Debug
		write(1,"Arg:",sizeof("Arg:"));
		write(1,Token,strlen(Token));
		write(1,"\n",sizeof("\n"));
#endif
		Args[Counter++] = Token;
		Token = strtok(null, " \n\r");
	}
	Args[Counter] = null;
}

int main()
{
	char InputBuff[Lab3InputBuffLength] = {0};
	char *Args[Lab3ArgumentsCount] = {0};

	go:

	if(!getcwd(InputBuff, Lab3InputBuffLength)) {
		write(1, "Error: Current directory path too long.", 38);
		return Lab3ErrorCWDTooLong;
	}

	strcat(InputBuff, ">:");
	write(1, InputBuff, strlen(InputBuff));

	memset(InputBuff, 0, Lab3InputBuffLength);
	read(2, InputBuff, Lab3InputBuffLength);
	MakeArgs(Args, InputBuff);

	switch(fork()) {
		case -1:
			write(1, "Error: Unable to fork process." ,29);
			return Lab3ErrorForking;
		case 0:
			if(strcmp(Args[0], "cd"))
				execvp(Args[0],Args);
			write(1, "Warning: Execution failed.\n", 27);
			break;
		default:
			if(!strcmp(Args[0], "cd"))
				chdir(Args[1]);
			wait(null);
			goto go;
	}

	return 0;
}
