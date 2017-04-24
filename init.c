#include "ucode.c"

int console;

main()
{
	int in, out;
	in = open("/dev/tty0", O_RDONLY);
	out = open("/dev/tty0", O_WRONLY);
	printf("INIT: fork a login process in console\n");
	console = fork();
	if(console)
	{
		parent();
	}
	else
	{
		login();
	}
	
	return 0;
}

int login()
{
	exec("login /dev/tty0");
	return 0;
}

int parent()
{
	int pid, status;
	
	while(1)
	{
		printf("INIT: wait for ZOMBIE child\n");
		pid = wait(&status);
		
		if (pid == console)
		{
			printf("INIT: forks a new console login\n");
			console = fork();
			
			if (console)
			{
				continue;
			}
			else
			{
				login();
			}
		}
		
		printf("INIT: buried an orphan child %d\n", pid);
	}
	
	return 0;
}
