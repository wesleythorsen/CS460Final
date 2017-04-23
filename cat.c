#include "ucode.c"

main(int argc, char *argv[])
{
	int infd = 0, outfd = 1;
	char buf[1024];
	char cwd[128];
	char f1[128];
	int n, i;
	
	printf("\n******************** Wes's cat ********************\n");

	if (argc > 2)
	{
		printf("error - argc > 2\nProgram usage: cat [filename]\n");
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
	
	n = read(infd, buf, 1024);
	while(n)
	{		
		for (i = 0; i < n; i++)
		{
			(buf[i] == '\n') ? printf("\n") : printf("%c", buf[i]);
		}
		
		n = read(infd, buf, 1024);
	}
	
	close(infd);
	close(outfd);
	
	return 0;
}



