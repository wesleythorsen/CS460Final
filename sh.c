#include "ucode.c"

main(int argc, char *argv[])
{
	char line[128], command[128], *tokens[128], cwd[128];
	int inputBad;
	int pid, status;

	while(1)
	{
		getcwd(cwd);
	
		// qet user input
		printf("%s $ ", cwd);
		gets(line);
		
		// remove leading and trailing spaces
		trimEnds(line, ' ', command);
		//printf("line: '%s'\n", line);
		
		// validate command syntax
		inputBad = validateSyntax(command);
		if (inputBad)
		{
			printf("Invalid input!\n");
			continue;
		}
		
		tokenize2(line, tokens);
		//printf("t0: %s\nt1: %s\n", tokens[0], tokens[1]);
		
		if (strcmp(tokens[0], "cd") == 0)
		{
			printf("changing dir to '%s'\n", tokens[1]);
			status = chdir(tokens[1]);
			continue;
		}
		else if (strcmp(tokens[0], "pwd") == 0)
		{
			printf("%s\n", cwd);
			continue;
		}
		else if (strcmp(tokens[0], "logout") == 0)
		{
			exit(0);
		}
		else if (strcmp(tokens[0], "exit") == 0)
		{
			exit(0);
		}
		
		//printf("%s\n", command);
		
		pid = fork();
		
		if (pid)
		{
			//printf("parent %d forks child %d\n", getpid(), pid);
			pid = wait(&status);
			//printf("child %d dies, parent %d continues\n", pid, getpid());
		}
		else
		{
			do_pipe(command, 0);
		}
	}
}

int do_pipe(char *line, int *pd)
{
	char head[128], tail[128];
	int lpd[2];
	int p, pid;

	if (pd)
	{
		close(pd[0]);
		dup2(pd[1], 1);
		close(pd[1]);
	}
	
	if (hasPipe(line))
	{
		splitPipe(line, head, tail);
		//printf("head: %s\ntail: %s\n", head, tail);
		
		p = pipe(&lpd);
		if (p < 0)
		{
			printf("unable to create pipe!\n");
			exit(1);
		}
		
		pid = fork();
		if (pid < 0)
		{
			printf("unable to fork a child!\n");
			exit(1);
		}
		if (pid)
		{
			//printf("parent %d forks child %d\n", getpid(), pid);
			close(lpd[1]);
			dup2(lpd[0], 0);
			close(lpd[0]);
			do_command(tail);
		}
		else
		{
			do_pipe(head, lpd);
		}		
	}
	else
	{
		do_command(line);
	}	
}

int do_command(char *line)
{
	char head[64], tail[64];
	int redirType, retVal;
	
	redirType = splitRedirect(line, head, tail);
	
	if (redirType == 0) // no redirect
	{
		//printf("proc %d executing '%s'\n", getpid(), head);
	}
	else if (redirType == -1) // < input reditect
	{
		close(0);
		retVal = open(tail, O_RDONLY);
		if (retVal < 0) 
		{
			printf("failed to open '%s' for read!\n", tail);
			exit(1);
		}
		
		//printf("proc %d executing '%s' with input from '%s'\n", getpid(), head, tail);
	}
	else if (redirType == 1) // > output redirect
	{
		close(1);
		retVal = open(tail, O_WRONLY|O_CREAT);
		if (retVal < 0) 
		{
			printf("failed to open '%s' for write/create!\n", tail);
			exit(1);
		}
		
		//printf("proc %d executing '%s' into file '%s'\n", getpid(), head, tail);
	}
	else if (redirType == 2) // >> output append redirect
	{
		close(1);
		retVal = open(tail, O_WRONLY|O_CREAT|O_APPEND);
		if (retVal < 0) 
		{
			printf("failed to open '%s' for write/create/append!\n", tail);
			exit(1);
		}
		
		//printf("proc %d executing '%s' appending to file '%s'\n", getpid(), head, tail);
	}
	
	
	retVal = exec(head);
	if (retVal < 0) 
	{
		printf("exec(%s) failed!\n", head);
		exit(1);
	}
	exit(0);
}

// returns:
// 		 0 - no redirect
//		-1 - < redirect
//		 1 - > redirect
//		 2 - >> redirect
int splitRedirect(char *in, char *head, char *tail)
{
	char buf[128];
	char *cp;
	int type = 0;
	
	strcpy(buf, in);
	cp = buf;
	
	while ((*cp != '>') && (*cp != '<') && (*cp != 0))
	{
		cp++;
	}
	
	if (*cp == '>') type = 1;
	if (*cp == '<') type = -1;
	
	*cp = 0;
	cp++;
	
	if (*cp == '>') // double >> redirect
	{
		type = 2;
		*cp = 0;
		cp++;
	}
	
	trimEnds(buf, ' ', head);
	trimEnds(cp, ' ', tail);
	
	return type;
}

int splitPipe(char *in, char *head, char *tail)
{
	char buf[128];
	char *cp1, *cp2;
	strcpy(buf, in);
	cp1 = buf;
	cp2 = buf;
	
	while (*cp2 != 0)
	{
		if (*cp2 == '|') cp1 = cp2;
		cp2++;
	}
	
	*cp1 = 0;
	cp1++;
	
	trimEnds(buf, ' ', head);
	trimEnds(cp1, ' ', tail);
	
	return 0;
}

int tokenize2(char *str, char *tokens[])
{
	int i = 0;
	char *t0;// = &(tokens[0][0]);
	char *t1;// = &(tokens[1][0]);
	char *cptr = str;
		
	while (*cptr != 0)
	{
		tokens[i] = 0;
		while (*cptr == ' ')
		{
			*cptr = 0;
			cptr++;
		}
		tokens[i] = cptr;
		i++;
		while ((*cptr != ' ') && (*cptr != 0)) cptr++;
	}
	
	return i;
}

int trimEnds(char *str, char trimChar, char *outbuf)
{
	char *cp1, *cp2;
	cp1 = str;
	*outbuf = 0;
	while (*cp1 == trimChar) cp1++;
	cp2 = cp1;
	while (*cp2) cp2++;
	cp2--;
	while (*cp2 == trimChar) cp2--;
	cp2++;
	*cp2 = 0;
	
	strcpy(outbuf, cp1);
	
	return 0;
}

int hasPipe(char *cp)
{
	while(*cp)
	{
		if (*cp == '|') return 1;
		cp++;
	}
	return 0;
}

int validateSyntax(char *in)
{
	// This funtion is a DFA for validating IO redirection and pipe syntax
	//
	// NOTE: This only validates the SYNTAX! 
	//		 Won't detect invalid function or file names
	//
	// ALSO NOTE: Known bug: "a | b < c | x" is valid (shouldn't be).
	//			  When designing DFA I forgot '<' had to only be in
	//			  the first command. In this DFA, '<' can be anywhere 
	//		 	  in the command chain and the DFA will evaluate to a 
	//			  valid state (assuming all other syntax rules are followed).
	//
	// Transitions (alphabet) are: 
	//					A  [a-zA-Z0-9/.]
	//				   ' ' (space, \n, \r) - these are ignored however (will not cause transition)
	//					>
	//					|
	//					<
	//			(any other chars are rejected, DFA will evaluate to invalid)
	//
	// States (Q) are: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	//
	// Start State (q0): 1
	//
	// Accept States (F) are 2, 4, 9
	//
	// Transition funtion implemented below:
	
	int curState = 1;
	char trans;
	
	while (*in)
	{
		// Determine transition type:
		if( ((*in > 47) && (*in < 58)) ||  // number 0-9
			((*in > 64) && (*in < 91)) ||  // uppercase A-Z
			((*in > 96) && (*in < 123)) || // lowercase a-z
			(*in == '/') ||					
			(*in == '.'))  
		{
			trans = 'A';
		}
		else if((*in == ' ') ||
				(*in == '\n') ||
				(*in == '\r'))
		{
			// NOTE: this type of transition is ignored in all states
			// (not really a transition, just move to next char)
			trans = ' ';
		}
		else
		{
			// >, |, <, or other character
			// if this isn't a > | or <, it will get rejected in the switch statement
			trans = *in;
		}
		
		//printf(" %d -'%c'->", curState, trans);
		
		switch (trans)
		{
			case ' ':
				// ignore spaces and newlines
				break;
			case 'A':
				if (curState == 1) curState = 2;
				else if (curState == 2) curState = 2;
				else if (curState == 3) curState = 2;
				else if (curState == 4) curState = 4;
				else if (curState == 5) curState = 4;
				else if (curState == 6) curState = 2;
				else if (curState == 7) curState = 9;
				else if (curState == 8) curState = 9;
				else if (curState == 9) curState = 9;
				else if (curState == 10) curState = 10;
				break;
			case '>':
				if (curState == 1) curState = 10;
				else if (curState == 2) curState = 7;
				else if (curState == 3) curState = 10;
				else if (curState == 4) curState = 10;
				else if (curState == 5) curState = 10;
				else if (curState == 6) curState = 10;
				else if (curState == 7) curState = 8;
				else if (curState == 8) curState = 10;
				else if (curState == 9) curState = 10;
				else if (curState == 10) curState = 10;
				break;
			case '|': 
				if (curState == 1) curState = 10;
				else if (curState == 2) curState = 6;
				else if (curState == 3) curState = 10;
				else if (curState == 4) curState = 3;
				else if (curState == 5) curState = 10;
				else if (curState == 6) curState = 10;
				else if (curState == 7) curState = 10;
				else if (curState == 8) curState = 10;
				else if (curState == 9) curState = 10;
				else if (curState == 10) curState = 10;
				break;
			case '<':
				if (curState == 1) curState = 10;
				else if (curState == 2) curState = 5;
				else if (curState == 3) curState = 10;
				else if (curState == 4) curState = 10;
				else if (curState == 5) curState = 10;
				else if (curState == 6) curState = 10;
				else if (curState == 7) curState = 10;
				else if (curState == 8) curState = 10;
				else if (curState == 9) curState = 10;
				else if (curState == 10) curState = 10;
				break;
			default:
				// Invalid character encountered
				printf("Invalid char: %c\n", trans);				
				curState = 10;
				break;
		}
		
		// State 10 means a syntax error has been encountered 
		// no need to continue to evaluate DFA, result will alway be 10
		if (curState == 10) break; 
		
		in++;
	}
	
	//printf(" %d \n", curState);
	
	if ((curState == 2) || 
		(curState == 4) || 
		(curState == 9))
	{
		return 0;
	}
	
	return curState;
}



