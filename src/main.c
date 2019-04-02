//DEVELOPMENT VERSION

#include <stdio.h> //IO
#include <stdlib.h> //MEM
#include <string.h> //MEM
#include <getopt.h> //CMD ARGS
#include <sys/types.h> //
#include <unistd.h>
#include "CheckOutput.h"

/* Boolean Value Definition */
#if __STDC_VERSION__ == 199901L
	/* Is C99 */
	#include <stdbool.h>
#else
	/* Is Not C99 */
	typedef enum { false, true } bool;
#endif

int main (int argc, char *argv[])
{
	/* Boolean Flags */
	bool inc = false,
	     dec = false,
	     set = false;

	/* Integers and Floats */
	int option = -1,
	    i_pct;
	double pct_b;
	
	/* Chars */
	char *cvalue = NULL;
	

	/* Get Information */
	char *buf = (char*) malloc (sizeof(char) * 16); //Temporary buffer
	char c; //Temporary character
	//File 1
	FILE *f = fopen("/sys/class/backlight/intel_backlight/max_brightness", "r");
	while((c =fgetc(f))!=EOF)
		sprintf(buf +strlen(buf), "%c", c);
	const int MAX_B = atoi (buf);
	fclose(f);
	//File 2
	memset(buf, 0x00, 16);
	f = fopen("/sys/class/backlight/intel_backlight/brightness", "r");
	while((c=fgetc(f))!=EOF)
		sprintf(buf + strlen(buf), "%c", c);
	fclose(f);
	const int CUR_B = atoi(buf);
	free(buf);
	
	/* Calculate Current Percent Brightness */
	pct_b = (double)(CUR_B * 100)/MAX_B;
	
	/* Parse Arguments */
	while ((option = getopt (argc, argv, "i:d:s:q")) != -1)
	{
		switch (option)
		{
			case 'q':
				printf ("Percent Brightness: %.2f%%\n", pct_b);
				return 0;
				break;
			case 's':
				set = true;
				cvalue = optarg;
				break;
			case 'i':
				inc = true;
				cvalue = optarg;
				break;
			case 'd':
				dec = true;
				cvalue = optarg;
				break;
			default:
				fprintf(stderr, "%s: Invalid option -- %c\n", argv[0], option);
				return -1;
		}
	}
	
	/* Check If Is Root And Has Arguments */
	if(argc < 2 || geteuid() != 0)
	{
		if(argc < 2)
		{	
			fprintf(stderr, "You have %d arguments, but you need at least 1\n", argc - 1);
			return -1;
		}
		if(geteuid() != 0)
		{
			fprintf(stderr, "You must be root to run this program with those options.\n");
			return -1;
		}
	}

	/* If User Wants To Increment Brightness */
	if (inc)
	{
		pct_b += atof(cvalue);
		if (pct_b > 100)
			pct_b = 100;
	} else if (dec) {
		pct_b -= atof(cvalue);
		if (pct_b < 0)
			pct_b = 0;
	} else if (set) {
		pct_b = atof(cvalue);
		if (pct_b > 100)
			pct_b = 100;
		else if (pct_b < 0)
			pct_b = 0;
	}

	/* Calculate Integer Brightness */
	i_pct = (int)((pct_b/100) * MAX_B);

	/* Set Brightness */
	FILE *fptr = fopen ("/sys/class/backlight/intel_backlight/brightness", "w");
	fprintf (fptr, "%d", i_pct);
	fclose (fptr);

}
