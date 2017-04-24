#include "ucode.c"

main(int argc, char *argv[])
{
	int infd = 0, outfd = 1;
	char buf[1024];
	char cwd[128];
	char f1[128], f2[128];
	char *cptr;
	int n, i;
	
	//printf("\n******************** Wes's l2u ********************\n");

	if (argc > 3)
	{
		printf("error - argc > 3\nProgram usage: l2u [f1 [f2]]\n");
		return;
	}
	
	getcwd(cwd);
	f1[0] = 0;
	f2[0] = 0;
	
	if (argc > 1)
	{
		if (argv[1][0] != '/') strcpy(f1, cwd);		
		strcat(f1, argv[1]);
		
		//printf("infile:  %s\n", f1);
		
		infd = open(f1, O_RDONLY);
	}
	if (argc > 2)
	{
		if (argv[2][0] != '/') strcpy(f2, cwd);		
		strcat(f2, argv[2]);
		
		//printf("outfile: %s\n", f2);
		
		outfd = open(f2, O_WRONLY|O_CREAT|O_TRUNC);
	}
	
	//printf(" in: %d\nout: %d\n", infd, outfd);
	
	if ((infd < 0) || (outfd < 0))
	{
		printf("error - cannot open file(s)\n");
	}
	
	n = read(infd, buf, 1024);
	while(n)
	{
		cptr = &buf[0];
		while(cptr < &buf[n])
		{
			if ((*cptr > 96) && (*cptr < 123))
			{
				*cptr -= 32;
			}
			cptr++;
		}
		
		if (outfd == 1)
		{
			for (i = 0; i < n; i++)
			{
				(buf[i] == '\n') ? printf("\n") : printf("%c", buf[i]);
			}
		}
		else
		{
			write(outfd, buf, n);
		}
		
		n = read(infd, buf, 1024);
	}
	
	close(infd);
	close(outfd);
	
	return 0;
}





