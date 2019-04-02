#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "CheckOutput.h"


int main()
{
	char *a = (char*) malloc(sizeof(char) * 64),
	     *b = (char*) malloc(sizeof(char) * 64),
	     *c = (char*) malloc(sizeof(char) * 64);
	memset(a, 0x00, 64);
	memset(b, 0x00, 64);
	memset(c, 0x00,64);
	char* cmd = "ls -la";
	size_t size = 64;
	/* COn -> Memory Safe. Won't send SIGSEGV */
	COn(cmd, a, size);
	printf("--COn--\n%s\n\n", a);
	free(a);
	
	/* sCOn -> Memory Safe. Won't send SIGSEGV */
	sCOn(cmd, b, size);
	printf("\n--sCOn--\n%s\n\n", b);
	free(b);
	
	/* CO -> Not Memory Safe. Will almost certainly send SIGSEGV */
	CO(cmd, c);
	printf("--CO--\n%s\n\n", c);
	free(c);

	/* Sends a general protection segfault here */
	/* Caused by invalid writes into c after it has already been filled */
}
