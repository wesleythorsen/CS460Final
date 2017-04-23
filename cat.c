#include "ucode.c"

main(int argc, char *argv[])
{
	int infd = 0, outfd = 1;
	char buf[1024];
	char cwd[128];
	char *cptr;
	int n, i;
	
	if (argc > 2)
	{
		printf("error - argc > 2\nUsage: cat [filename]\n");
		return 1;
	}
	
	if (argc == 2)
	{
		// if filename was given, open file for read
		infd = open(argv[1], O_RDONLY);
	}
	
	if (infd < 0)
	{
		printf("error - cannot open file '%s'\n", argv[1]);
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
