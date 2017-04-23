#include "ucode.c"

char *tty;
int in, out, err;
int pwfd;
char username[32], password[32], udname[32], uhomedir[32], uprogram[32];
int upid, ugid;

main(int argc, char *argv[])
{
	char buf[1024];
	tty =  argv[1];

	close(0); 
	close(1); 
	close(2); // login process may run on different terms
	
	in  = open(tty, O_RDONLY);	
	out = open(tty, O_WRONLY);
	err = open(tty, O_WRONLY);

	settty(tty);   // store tty string in PROC.tty[] for putc()

	// NOW we can use printf, which calls putc() to our tty
	printf("LOGIN : open %s as stdin, stdout, stderr\n", tty);

	// ignore Control-C interrupts so that 
	// Control-C KILLs other procs on this tty but not the main sh
	signal(2,1);  

	while(1)
	{
		printf("Enter username: ");
		gets(username);
		printf("Enter password: ");
		gets(password);

		if(verifyLogin(username, password))
		{
			// password and username are valid
			chuid(upid);
			chdir(uhomedir);
			exec(uprogram);
		}
		else
		{
			printf("login failed, try again\n");
		}
	}
}

int verifyLogin(char *uname, char *pword)
{
	int valid = 0;
	char buf[1024];
	char *line, *un, *pw, *sptr;
	int r;
	pwfd = open("/etc/passwd", O_RDONLY);
	
	if (pwfd < 0)
	{
		printf("cannot open '/etc/passwd': exit(1)\n");
		return 0;
	}
	
	r = read(pwfd, buf, 1024);
	line = &buf[0];
	
	while (line < &buf[r])
	{
		un = line;
		while (*line != ':') line++;
		*line = '\0'; line++;
		
		pw = line;
		while (*line != ':') line++;
		*line = '\0'; line++;
		
		if ((strcmp(un, uname) == 0) && (strcmp(pw, pword) == 0))
		{
			printf("username and password valid!\n");
			valid = 1;
			
			// find user pid
			sptr = line;
			while (*line != ':') line++;
			*line = '\0'; line++;
			upid = atoi(sptr);
		
			// find user gid
			sptr = line;
			while (*line != ':') line++;
			*line = '\0'; line++;
			ugid = atoi(sptr);
		
			// find user display name
			sptr = line;
			while (*line != ':') line++;
			*line = '\0'; line++;
			strcpy(udname, sptr);
		
			// find user home directory
			sptr = line;
			while (*line != ':') line++;
			*line = '\0'; line++;
			strcpy(uhomedir, sptr);
		
			// find user program
			sptr = line;
			while (*line != '\n') line++;
			*line = '\0'; line++;
			strcpy(uprogram, sptr);
			
			// print user info:
			printf("username: %s\npassword: %s\npid: %d\ngid: %d\ndisplay name: %s\nhome dir: %s\nprogram: %s\n", 
				username, password, upid, ugid, udname, uhomedir, uprogram);
			
			break;
		}
		else
		{
			//printf("%s != %s && %s != %s\n", uname, un, pword, pw);
		}
		
		while (*line++ != '\n');
	}
	
	close(pwfd);
	
	return valid;
}









