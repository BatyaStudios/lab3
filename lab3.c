#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define Lab3InputBuffLength 512
#define Lab3ArgumentsCount 16

#define Lab3ErrorForking 1
#define Lab3ErrorCWDTooLong 2
#define Lab3ErrorSig 3

#define Lab3WaitOn 1
#define Lab3WaitOff 0

#define Lab3Debug

#ifndef null
#define null NULL
#endif

char Wait = Lab3WaitOn;
int ProcessID;
int ProcessChildID;

void sig_handler(int signo)
{
	if (getpid() == ProcessChildID)
		if (Wait == Lab3WaitOn)
			kill(ProcessChildID, SIGINT);
}

void MakeArgs(char **Args, char *Line)
{
	char *Token = strtok(Line, "\n\r ");
	int Counter = 0;

	while (Token) {
#ifdef Lab3Debug
		write(1, "Arg:", sizeof("Arg:"));
		write(1, Token, strlen(Token));
		write(1, "\n", sizeof("\n"));
#endif
		Args[Counter++] = Token;
		Token = strtok(null, "\n\r ");
	}
	Args[Counter] = null;
}

int main(void)
{
	char InputBuff[Lab3InputBuffLength] = {0};
	char *Args[Lab3ArgumentsCount] = {0};
	int i = 0;
	char *R;

	if (signal(SIGINT, sig_handler) == SIG_ERR)
		return Lab3ErrorSig;

go:
	if (!getcwd(InputBuff, Lab3InputBuffLength)) {
		write(1, "Error: Current directory path too long.\n", 38);
		return Lab3ErrorCWDTooLong;
	}

	strcat(InputBuff, ">:");
	write(1, InputBuff, strlen(InputBuff));

	memset(InputBuff, 0, Lab3InputBuffLength);
	read(2, InputBuff, Lab3InputBuffLength);

	R = strrchr(InputBuff, '*');
	if (!R) {
#ifdef Lab3Debug
		write(1, "Wait: On\n", sizeof("Wait: On\n"));
#endif
		Wait = Lab3WaitOn;
	} else {
#ifdef Lab3Debug
		write(1, "Wait: Off\n", sizeof("Wait: Off\n"));
#endif
		Wait = Lab3WaitOff;
		*R = 0;
	}

	MakeArgs(Args, InputBuff);

	ProcessID = fork();

	switch (ProcessID) {
	case -1:
		write(1, "Error: Unable to fork process.\n", 30);
		return Lab3ErrorForking;
	case 0:
		ProcessChildID = getpid();
		if (Wait == Lab3WaitOff)
			signal(SIGINT, SIG_IGN); //!!!
		execvp(Args[0], Args);

		if (strcmp(Args[0], "cd") && strcmp(Args[0], "quit"))
			write(1, "Error\n", sizeof("Error\n"));
		return 0; //???
	default:
		if (!strcmp(Args[0], "quit"))
			return 0;
		if (!strcmp(Args[0], "cd")) {
			chdir(Args[1]);
			goto go;
		}
		if (Wait == Lab3WaitOn)
			waitpid(ProcessID, null, 0);
		goto go;
	}
	return 0;
}
