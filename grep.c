#include "ucode.c"

main(int argc, char *argv[])
{
	int infd = 0, outfd = 1;
	char cwd[128];
	char f1[128];
	char *pattern;
	char line[256];
	int moreLines;
	
	//printf("\n******************** Wes's grep ********************\n");

	if (argc > 3)
	{
		printf("error - argc > 3\nProgram usage: grep pattern [filename]\n");
		return 1;
	}
	if (argc < 2)
	{
		printf("error - argc < 2\nProgram usage: grep pattern [filename]\n");
		return 1;
	}
	
	getcwd(cwd);
	f1[0] = 0;
	pattern = argv[1];
	
	if (argc == 3)
	{
		if (argv[2][0] != '/') strcpy(f1, cwd);		
		strcat(f1, argv[2]);
		infd = open(f1, O_RDONLY);
	}
	
	if (infd < 0)
	{
		printf("error - cannot open file '%s'\n", f1);
	}
	
	
	do {
		moreLines = getLine(infd, line);
		if (strstr(line, pattern) != NULL)
		{
			printf("%s\n", line);
		}
	} while (moreLines);
	
	close(infd);
	close(outfd);
	
	return 0;
}

int getLine(int fd, char *line)
{
	int n, i = 0;
	char c = 0;
	
	*line = 0;
	while((c != '\n') && (c != '\r'))
	{
		n = read(fd, &c, 1);
		if (n < 1) return 0;
		line[i] = c;
		i++;
		line[i] = 0;
	}
	line[i - 1] = 0; // remove the \n or \r character from string
					// its messing up the print statemenets
	
	return 1;
}




