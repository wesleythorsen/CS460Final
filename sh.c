#include "ucode.c"

main(int argc, char *argv[])
{
	char input[1024];
	int validInput;

	while(1)
	{
		printf("WES-CMD: ");
		gets(input);
		
		// validate syntax
		validInput = validateSyntax(input);
		if (validInput != 0)
		{
			printf("Invalid input syntax!\n", validInput);
			continue;
		}
		
		
		
	}
}

int validateSyntax(char *in)
{
	// This funtion is a DFA
	// NOTE: This only validates the SYNTAX! 
	//		 Won't detect invalid function or file names
	//
	// Transitions (alphabet) are: 
	//					A  [a-zA-Z0-9]
	//				   ' ' (space, \n, \r)
	//					>
	//					|
	//					<
	//			(any other chars are rejected)
	//
	// States (Q) are: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	//
	// Accept States (F) are 2, 4, 9
	//
	// Transition funtion implemented below:
	
	int curState = 1;
	char trans;
	
	while (*in)
	{
		// Determine transition type:
		if( ((*in > 47) && (*in < 58)) || // number 0-9
			((*in > 64) && (*in < 91)) || // uppercase A-Z
			((*in > 96) && (*in < 123)))  // lowercase a-z
		{
			trans = 'A';
		}
		else if((*in == ' ') ||
				(*in == '\n') ||
				(*in == '\r'))
		{
			// NOTE: this type of transition is ignored in all cases
			trans = ' ';
		}
		else
		{
			// > | < or other character
			// if this isn't a > | or <, it will get rejected in the switch statement
			trans = *in;
		}
		
		printf(" %d -'%c'->", curState, trans);
		
		switch (trans)
		{
			case ' ':
				break;
			case 'A':
				if (curState == 1) curState = 2;
				else if (curState == 2) curState = 2;
				else if (curState == 3) curState = 2;
				else if (curState == 4) curState = 4;
				else if (curState == 5) curState = 4;
				else if (curState == 6) curState = 2;
				else if (curState == 7) curState = 9;
				else if (curState == 8) curState = 9;
				else if (curState == 9) curState = 9;
				else if (curState == 10) curState = 10;
				break;
			case '>':
				if (curState == 1) curState = 10;
				else if (curState == 2) curState = 7;
				else if (curState == 3) curState = 10;
				else if (curState == 4) curState = 10;
				else if (curState == 5) curState = 10;
				else if (curState == 6) curState = 10;
				else if (curState == 7) curState = 8;
				else if (curState == 8) curState = 10;
				else if (curState == 9) curState = 10;
				else if (curState == 10) curState = 10;
				break;
			case '|': 
				if (curState == 1) curState = 10;
				else if (curState == 2) curState = 6;
				else if (curState == 3) curState = 10;
				else if (curState == 4) curState = 3;
				else if (curState == 5) curState = 10;
				else if (curState == 6) curState = 10;
				else if (curState == 7) curState = 10;
				else if (curState == 8) curState = 10;
				else if (curState == 9) curState = 10;
				else if (curState == 10) curState = 10;
				break;
			case '<':
				if (curState == 1) curState = 10;
				else if (curState == 2) curState = 5;
				else if (curState == 3) curState = 10;
				else if (curState == 4) curState = 10;
				else if (curState == 5) curState = 10;
				else if (curState == 6) curState = 10;
				else if (curState == 7) curState = 10;
				else if (curState == 8) curState = 10;
				else if (curState == 9) curState = 10;
				else if (curState == 10) curState = 10;
				break;
			default:
				// Invalid character encountered
				printf("Invalid char: %c\n", trans);				
				curState = 10;
				break;
		}
		
		// State 10 means a syntax error has been encountered 
		if (curState == 10) break; 
		
		in++;
	}
	
	printf(" %d \n", curState);
	
	if ((curState == 2) || 
		(curState == 4) || 
		(curState == 9))
	{
		return 0;
	}
	
	return curState;
}



