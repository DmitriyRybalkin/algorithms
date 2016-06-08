
/* This program reads in a directed graph from stdin
 * and either writes the vertices in topological order
 * or notes that it is not acyclic.
 *                                                     
 *   Input has the form:                               
 *                                                     
 *  6  -- number of vertices                           
 *  2 0  0 1   -- edges in free format                 
 *  0 4  0 5  2 5                                      
 *                                                     
 * The graph ends when stdin ends. Vertices are        
 * numbered starting at 0. There is a limit of 100 vertices.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAXNV 100    /* Maximum number of vertices */

typedef int digraph[MAXNV][MAXNV+1];
     /* For simplicity in this lab, a directed graph will be represented
        using a list of neighbours in each row of a matrix.  This
        is not space efficient for sparse digraphs, obviously.
        Each list is terminated by -1.  For example, the digraph
        above has the following in the upper left corner:

          1  4  5 -1      <- neighbours of 0   ( entries [0][0..3])
         -1               <- neighbours of 1   ( entry   [1][0])
          0  5 -1              .               ( entries [2][0..2]
         -1                    .
         -1                    .
         -1               <- neighbours of 5   (entry    [5][0])
     */

#define WHITE 0     /* Colours to use in the DFS */
#define GRAY  1
#define GREY  GRAY
#define BLACK 2

static void read_digraph(int *np, digraph g)
/* Read a digraph from stdin into g.
   Return the number of vertices in *np.
   Also print the graph.
*/

{
	int n,v,w,i;
	int next[MAXNV];   /* place to put next neighbour */

	if (scanf("%d",&n) != 1 || n <= 0 || n > MAXNV)
	{
	    fprintf(stderr,"No vertex count, or invalid value.\n");
	    exit(1);
	}

	for (v = 0; v < n; ++v)
	    next[v] = 0;           /* Initialise to empty */

	while (scanf("%d%d",&v,&w) == 2) /* scan a directed edge <v,w> */
	{
	    if (v < 0 || v >= n || w < 0 || w >= n)
	    {
		fprintf(stderr,"Illegal edge %d->%d\n",v,w);
		exit(1);
	    }

	    if (next[v] >= MAXNV)
	    {
		fprintf(stderr,"Too many edges for vertex %d\n",v);
		exit(1);
	    }

	    g[v][next[v]++] = w;    /* Insert new edge */
	}

	for (v = 0; v < n; ++v)
	    g[v][next[v]] = -1;     /* Mark the ends */

	*np = n;

	printf("n = %d\n",n);

	for (v = 0; v < n; ++v)
	{
	    printf("%4d : ",v);
	    for (i = 0; g[v][i] >= 0; ++i)
		printf(" %2d",g[v][i]);
	    printf("\n");
	}
}

int stack[MAXNV];
int si;

static void dfs_visit(digraph g, int v, int *colour)
/* Do a depth-first scan in digraph g starting
   at vertex v. */
{
	int i;
	colour[v] = GREY;

	for (i = 0; g[v][i] != -1; i++) {
	  if (colour[g[v][i]] == WHITE) {
	    dfs_visit(g,g[v][i],colour);
	  }
	  if (colour[g[v][i]] == GREY) {
	    printf("The digraph has a cycle\n");
	    exit(0);
	  }
	}
	stack[si++] = v;
	colour[v] = BLACK;
}


static void dfs(int n, digraph g)
/* Perform depth-first search on the digraph g,
   which has n vertices.  Then print the vertices
   in topological order or else report that the
   digraph has a cycle. 
*/

{
	int v;
	int colour[MAXNV];
	si = 0;

	for (v = 0; v < n; ++v)
	    colour[v] = WHITE;

	for (v = 0; v < n; ++v)
	    if (colour[v] == WHITE) dfs_visit(g,v,colour);

	v = n;
	printf("Topological order:");
	while (--v >= 0)
	  printf(" %d",stack[v]);
	printf("\n");

}

int main(int argc, char *argv[])
{
	digraph g;
	int n;

	read_digraph(&n,g);

	dfs(n,g);

        return 0;
}
