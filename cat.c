#include "ucode.c"

main(int argc, char *argv[])
{
	int infd = 0, outfd = 1;
	char buf[1024];
	char cwd[128];
	char f1[128];
	int n, i;
	
	char nl = '\n';
	
	//printf("\n******************** Wes's cat ********************\n");

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
		exit(1);
	}
	
	n = read(infd, buf, 1024);
	while(n)
	{		
		for (i = 0; i < n; i++)
		{
			//printf("%d", (i%10));
			
			
			
			if ((buf[i] == '\n') || (buf[i] == '\r')) write(outfd, &nl, 1);
			//else if (buf[i] == '\r');
			else write(outfd, &buf[i], 1);
			
			//if (buf[i] == '\n') printf("\n");
			//else if (buf[i] == '\r');
			//else printf("%c", buf[i]);
			
			//((buf[i] == '\n') || (buf[i] == '\r')) ? printf("\n") : printf("%c", buf[i]);
			
			//(buf[i] == '\n') ? putc('\n') : putc(buf[i]);
			
			//if (buf[i] == '\n') putc('\r');
			//else putc(buf[i]);
		}
		
		n = read(infd, buf, 1024);
	}
	
	close(infd);
	close(outfd);
	
	return 0;
}



