#include "ucode.c"

int infd, outfd;

main(int argc, char *argv[])
{
	char buf[1024];
	char cwd[128];
	char *cptr;
	int n, i;
	
	getcwd(cwd);
	
	printf("********** Wes's l2u() **********\n");

	if (argc > 3)
	{
		printf("error - argc != 3\nProgram usage: l2u [f1 f2]\n");
		return;
	}
	
	// set IO file descriptors to stdin and stdout
	infd = 0;
	outfd = 1;
	if (argc > 1)
	{
		infd = open(argv[1], O_RDONLY);
	}
	if (argc > 2)
	{
		outfd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC);
	}
	
	printf(" in: %d\nout: %d\n", infd, outfd);
	
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
