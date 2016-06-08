

/* Find the best alignment of two RNA sequences.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 100  /* Maximum length of X and Y */

char *paddedx, *paddedy;

static int score(char *x, char *y /*char *paddedx, char *paddedy*/)
/*   Find the best alignment of *x and *y.
 *   Return the score as function value and give the alignment
 *   by putting the padded strings into *paddedx and *paddedy.
 */
{
    int C[MAXLEN+1][MAXLEN+1];   /* Array of best scores */
    int W[MAXLEN+1][MAXLEN+1];   /* Array specifying alignment:
				0 = used only for (0,0)
				1 = X aligned with gap
				2 = Y aligned with gap
				3 = X aligned with Y  */
    int m,n;      /* The lengths of X and Y, respectively */
    int i,j,pos;
    int answer,sofar,paddedlen, l;
    //char *paddedx, *paddedy;

    m = strlen(x);    /* length of the string x */
    n = strlen(y);    /* length of the string y */
    
    
    if (m > MAXLEN || n > MAXLEN)
    {
	fprintf(stderr,"Error: X or Y is too long\n");
	exit(1);
    }

    for (i = 0; i <= m; ++i)
    for (j = 0; j <= n; ++j)
    {
        sofar = -2*(m+n+1);   /* Smaller than any possible score,
			        remembers best value so far */

	if (i == 0 && j == 0)
	{
	    C[i][i] = 0;
	    W[i][i] = 0;
	    sofar = 0;
	}

	if (i > 0 && C[i-1][j]-2 > sofar)
	{
	    C[i][j] = C[i-1][j]-2;
	    W[i][j] = 1;
	    sofar = C[i][j];
	}

	if (j > 0 && C[i][j-1]-2 > sofar)
	{
	    C[i][j] = C[i][j-1]-2;
	    W[i][j] = 2;
	    sofar = C[i][j];
	}

	if (i > 0 && j > 0 && x[i-1] == y[j-1] && C[i-1][j-1]+1 > sofar)
	{
	    C[i][j] = C[i-1][j-1]+1;
	    W[i][j] = 3;
	    sofar = C[i][j];
	}

	if (i > 0 && j > 0 && x[i-1] != y[j-1] && C[i-1][j-1]-1 > sofar)
	{
	    C[i][j] = C[i-1][j-1]-1;
	    W[i][j] = 3;
	    sofar = C[i][j];
	}
        printf("C(%d,%d)=%3d  W(%d,%d)=%d\n",
		i,j,C[i][j],i,j,W[i][j]);  /* temp. for debugging */
    }

    answer = C[m][n];

    /* Calculated the length of the padded strings */

    i = m; j = n;
    paddedlen = 0;
    while (i > 0 || j > 0)
    {
	++paddedlen;
	if      (W[i][j] == 1)
	    --i;
	else if (W[i][j] == 2)
	    --j;
	else
	{
	    --i;
	    --j;
	}
    }
    printf("Length of padded strings = %d\n",paddedlen); /* debug */

    /* make the padded strings
     *  THIS IS THE PART YOU NEED TO DO
     */

     paddedx=(char*)calloc(paddedlen, sizeof(char));
     paddedy=(char*)calloc(paddedlen, sizeof(char));

     if(paddedx==NULL || paddedy==NULL)
     {
     printf("\n memory allocation error \n");
     return;
     }
    /* Don't forget to null-terminate the padded strings */
      i = m; j = n;
    pos=paddedlen-1;
    {
	while(i>0||j>0)
	{
	    --pos;
	    
	    switch(W[i][j])
	    {
	    case 1: { paddedx[pos]=x[i]; paddedy[pos]='-';  --i; } break;
	    case 2: { paddedy[pos]=y[j]; paddedx[pos]='-'; --j; } break;
	    case 3: { paddedx[pos]=x[i]; paddedy[pos]=y[j];--i; --j;} break;
		    
	    default: {--i; --j;}
      
	    }
	}
    }

    paddedx[paddedlen] = '\0';
    paddedy[paddedlen] = '\0';

    return answer;
}

int main(int argc, char *argv[])
{
    char *x,*y;
    int max_score;
    //char paddedx[2*MAXLEN+1],paddedy[2*MAXLEN+1];

    /*if (argc != 3)
    {
        fprintf(stderr,"Usage: two DNA sequences string1 string2\n");
        exit(1);
    }*/

    x = "GACGGATTAGAA";
    y = "GATCGGAATAG";

    max_score = score(x,y /*paddedx,paddedy*/);

    printf("score(%s,%s)=%d\n", x, y, max_score);

    printf("Example of optimal alignment:\n  %s\n  %s\n", paddedx, paddedy);

    free(paddedx);
    free(paddedy);

    return 0;
}
