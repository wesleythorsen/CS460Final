#include "ucode.c"

main(int argc, char *argv[])
{
	int infd, outfd;
	char buf[1024];
	char cwd[128];
	char f1[128], f2[128];
	int n;
	
	//printf("\n******************** Wes's cp ********************\n");

	if (argc != 3)
	{
		printf("error - argc != 3\nProgram usage: cp f1 f2\n");
		return;
	}
	
	getcwd(cwd);
	f1[0] = 0;
	f2[0] = 0;
	
	if (argv[1][0] != '/') strcpy(f1, cwd);		
	strcat(f1, argv[1]);
	infd = open(f1, O_RDONLY);
	
	if (argv[2][0] != '/') strcpy(f2, cwd);		
	strcat(f2, argv[2]);
	outfd = open(f2, O_WRONLY|O_CREAT|O_TRUNC);
		
	if ((infd < 0) || (outfd < 0))
	{
		printf("error - cannot open file(s)\n");
	}
	
	n = read(infd, buf, 1024);
	while(n)
	{		
		write(outfd, buf, n);
		n = read(infd, buf, 1024);
	}
	
	close(infd);
	close(outfd);
	
	return 0;
}





