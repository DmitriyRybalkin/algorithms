
/* Dijkstra algorithm
 * Author: Dmitriy Rybalkin <dmitriy.rybalkin@gmail.com>
 * Created: 12.01.2010
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* Some code probably useful for the implementation of
 * Dijkstra's algorithm for one-many shortest paths.
 * 
 * If there is one parameter, it is an input file.
 * If there are no parameters, the input file is "test.data".
*/

#define  NIL        (-1)
#define  MAXNODES   100
#define  MAXLENGTH  1000
#define  INFINITY   (MAXNODES*MAXLENGTH)
#define  MAXEDGES   (MAXNODES*MAXNODES)
#define FALSE 0
#define TRUE  1

  
typedef struct nodestruct
{
  int x;
  int weight;
  struct nodestruct *next;
} node;

int N;


void printPath(int *pred, int s, int v)
/*
 * Prints out a path from s to v, if one exists. Else, prints 'no path from
 * s to v exists'. pred is the predecessor array, with a value of
 * pred[v] = NIL indicating that v has no predecessor.
 */
{
    if (v == s)
        printf("%2d", s);
    else if (pred[v] == NIL)
        printf("No path exists from %d to %d (THIS CAN\'T HAPPEN)\n", s, v);
          /* The case of no path should be caught in main() already. */
    else  
    {
        printPath(pred,s,pred[v]);
        printf("%2d", v);
    }
}


void initSingleSource(int *pred, int *dist, int s, int n)
/*
 * Initializes each vertex in the predecessor array to have a value of NULL.
 * Also sets the initial shortest-path estimate for each vertex to
 * INFINITY, except for the source vertex s which is given a value of 0.
 */
{
    int i;

    for (i = 0; i < n; ++i)
    {
        pred[i] = NIL;
        dist[i] = INFINITY;
    }
    dist[s] = 0;
}


void relax(int *pred, int *dist, int u, int v, int len)
/*
 * Relaxes the edge (u,v), which has length len.
 *
 */
{
    if (dist[v] > dist[u] + len)
    {
        dist[v] = dist[u] + len;
        pred[v] = u;
    }
}
 
void read_digraph(node **digraph, int *n, char *filename)
 /* read the input data and produce adjacency list of G=(V,E) 
  * assume that the first item in the input file is the number
  * of vertices in G, and the rest consists of integer triples
  * u v w, which means there is a directed edge in $G$ 
  * from u to v with weight w.
  *
  * Format of result:  *n = number of vertices
  *                   For i = 0..n-1, digraph[i] is NULL
  *                       if i has no edges coming out,  Otherwise
  *                       digraph[i] points to a simple linked list
  *                       of type node giving the edges coming out of i.
  */
{
    int i, j, w;
    node *p, *r;
    FILE *infile;

    infile = fopen(filename, "r");
    if (fscanf(infile, "%d", n) != 1 || *n < 1 || *n > MAXNODES)
    {
	fprintf(stderr,"Error: missing or illegal number of vertices\n");
	exit(1);
    }
   
    for (i = 0; i < *n; ++i) digraph[i] = NULL;

    while (fscanf(infile, "%d %d %d", &i, &j, &w) == 3)
    { 
        r = (node*)malloc(sizeof(node));
        r->x = j;
        r->weight = w;
        r->next = NULL;

        p = digraph[i];
        if (p == NULL)
	{
            digraph[i] = r;
        }
        else
	{
            while (p->next !=NULL) p=p->next;
            p->next = r;
        }
   }
   fclose(infile);
}


void Dijkstra_algorithm(int *pred, int *dist, int s, int n, node **digraph)
  /* find the shortest path from s to all other nodes 
   * using Dijkstra's algorithm 
   */
{
  int  i, ndone, mindist, besti;
  int S[MAXNODES];  /* S[i] = 1 if i is in the set S, 0 otherwise. */
  node* no;

  for (i = 0; i < n; ++i) S[i] = FALSE;   /* Initialise S to empty */

  for (ndone = 0; ndone < n; ndone++) {
    mindist = INFINITY;
    for(i = 0; i < n; ++i) {
      if (dist[i] < mindist && (S[i] == FALSE)) {
        mindist = dist[i];
        besti = i;
      }
    }
    S[besti] = TRUE;
    no = digraph[besti];
    while(no != NULL) {
      relax(pred,dist,besti,no->x,no->weight);
      no = no->next;
    }
  }
}


int main(int argc, char *argv[])
{

  int i,j,n,diam,e1,e2;
  node *digraph[MAXNODES];
  int pred[MAXNODES], dist[MAXNODES];
  char *filename;  
 
  if (argc == 1)      filename = "test.data";
  else if (argc == 2) filename = argv[1];
  else
  {
      fprintf(stderr,"Usage:  Dijkstra [file]\n");
      exit(1);
  }

  read_digraph(digraph, &n, filename);  

  diam = -1;
  for (j = 0; j < n; j++) {

    initSingleSource(pred, dist, j, n);

    /* find the shortest path from 0 to all other nodes */
    /* using Dijkstra's algorithm */

    Dijkstra_algorithm(pred, dist, j, n, digraph); 

    for (i = 0; i < n; ++i)
    {
        if (dist[i] >= INFINITY)
      printf("%d cannot be reached from %d\n",i,j);
        else
        {
            if (dist[i] > diam) {
              diam = dist[i];
              e1 = j;
              e2 = i;
            }
            printf("A shortest path from %d to %d is", j, i);
            printPath(pred, j, i);
            printf(" with length %d\n", dist[i]);
        }
    }
  }
  printf("The diameter is from %d to %d with length %d\n",e1,e2,diam);

  return 0;
}
