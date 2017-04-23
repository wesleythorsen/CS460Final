#include "ucode.c"

main(int argc, char *argv[])
{
	int infd = 0, outfd = 1;
	char cwd[128];
	char f1[128];
	int lines;
	char input;
	
	printf("\n******************** Wes's more ********************\n");

	if (argc > 2)
	{
		printf("error - argc > 2\nProgram usage: more [filename]\n");
		return 1;
	}
	
	getcwd(cwd);
	f1[0] = 0;
	
	if (argc == 2)
	{
		if (argv[1][0] != '/') strcpy(f1, cwd);		
		strcat(f1, argv[1]);
		infd = open(f1, O_RDONLY);
	}
	
	if (infd < 0)
	{
		printf("error - cannot open file '%s'\n", f1);
	}
	
	
	lines = 1;
	while(printLines(infd, lines))
	{		
		input = getc();
		if (input == ' ') lines = 20;
		else if ((input == '\n') || (input == '\r')) lines = 1;
		else if (input == 'q') break;
	}
	
	close(infd);
	close(outfd);
	
	return 0;
}

int printLines(int fd, int lines)
{
	int n;
	char c = 'x'; // x just means not \n or \r. 'x' is never printed
	
	while(lines--)
	{
		while((c != '\n') && (c != '\r'))
		{
			n = read(fd, &c, 1);
			if (n < 1) return 0;
			putc(c);
		}
		c = 'x'; // aka make c not equal to \n or \r 
	}
	
	return 1;
}




