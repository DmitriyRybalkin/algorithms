
/* Minimum Spanning Tree 
 * Author: Dmitriy Rybalkin <dmitriy.rybalkin@gmail.com>
 * Created: 20.02.2009
 */
#include <stdlib.h>
#include <stdio.h>

#define MAXSIZE 100
#define INFINITY 2


typedef struct {
    int item;  /* vertex number is used for the item. */
    long key;  /* distance is used as the key. */
} bheap_item_t;


typedef struct {
    bheap_item_t *a;
    int *p;
    int n;
    long key_comps;
} bheap_t;


bheap_t *heap_alloc(int n)
{
    bheap_t *h;

	/* Create the binary heap. */
    h = malloc(sizeof(bheap_t));
	
    h->a = calloc(n+1, sizeof(bheap_item_t));
    h->p = calloc(n, sizeof(int));
    h->n = 0;
    h->key_comps = 0;

    return h;
}

void heap_free(bheap_t *h)
{
    free(h->a);
    free(h->p);
    free(h);
}

int heap_min(bheap_t *h)
{
    /* The item at the top of the binary heap has the minimum key value. */
    return h->a[1].item;
}

void heap_insert(bheap_t *h, int item, double key)
{
    /* i - insertion point
     * j - parent of i
     * y - parent's entry in the heap.
     */
    int i, j;
    bheap_item_t y;

    /* i initially indexes the new entry at the bottom of the heap. */
    i = ++(h->n);

    /* Stop if the insertion point reaches the top of the heap. */
    while(i >= 2) {
        /* j indexes the parent of i.  y is the parent's entry. */
	j = i / 2;
	y = h->a[j];

        /* the correct insertion point is when the items key is >= parent
         * Otherwise we move the parent down and insertion point up.
         */
	h->key_comps++;
	if(key >= y.key) break;

	h->a[i] = y;
	h->p[y.item] = i;
	i = j;
    }

    /* Insert the new item at the insertion point found. */
    h->a[i].item = item;
    h->a[i].key = key;
    h->p[item] = i;
}

void heap_siftup(bheap_t *h, int p, int q)
{
    /* y - the heap entry of the root.
     * j - the current insertion point for the root.
     * k - the child of the insertion point.
     * z - heap entry of the child of the insertion point.
     */
    int j, k;
    bheap_item_t y, z;

    /* Get the value of the root and initialise the insertion point and child.
     */
    y = h->a[p];
    j = p;
    k = 2 * p;

    /* shift-up only if there is a child of the insertion point. */
    while(k <= q) {

        /* Choose the minimum child unless there is only one. */
	z = h->a[k];
	if(k < q) {
		h->key_comps++;
		if(z.key > h->a[k + 1].key) z = h->a[++k];
        }

        /*stop if the insertion point for the root is in the correct place.
         * Otherwise the child goes up and the root goes down.  (i.e. swap)
         */
	if(y.key <= z.key) break;
	h->a[j] = z;
	h->p[z.item] = j;
	j = k;
	k = 2 * j;
 }

    /* Insert the root in the correct place in the heap. */
    h->a[j] = y;
    h->p[y.item] = j;
}

void heap_delete(bheap_t *h, int item)
{
    int n;
    int p;

    /* Decrease the number of entries in the heap and record the position of
     * the item to be deleted.
     */
    n = --(h->n);
    p = h->p[item];

    /* Heap needs adjusting if the position of the deleted item was not at the
     * end of the heap.
     */
    if(p <= n) {
        /* put the item at the end of the heap in the place of the deleted
         * item and sift-up or sift-down to relocate it in the correct place in
         * the heap.
         */
    h->key_comps++;
    if(h->a[p].key <= h->a[n + 1].key) {
      h->a[p] = h->a[n + 1];
      h->p[h->a[p].item] = p;
      heap_siftup(h, p, n);
    } else {
            
      h->n = p - 1;
      heap_insert(h, h->a[n + 1].item, h->a[n+1].key);
      h->n = n;
      }
    }
}

void heap_decrease_key(bheap_t *h, int item, double new_key)
{
    int n;

    n = h->n;
	
    h->n = h->p[item] - 1;

    heap_insert(h, item, new_key);

    h->n = n;
}

int heap_size(void *h) {
    return ((bheap_t *)h)->n;
}

int heap_extract_min(void *h) {
    int v;
    v = heap_min((bheap_t *)h);
    heap_delete((bheap_t *)h, v);
    return v;
}

double **GRAPH;
double w[MAXSIZE][MAXSIZE];
double d[MAXSIZE];
int used[MAXSIZE];
int in_tree[MAXSIZE];
void *Q;

void initialize_graph(int dim)
{
    int size=dim;
    int i, j;
    //allocate graph matrix
    double **graph=(double**)calloc(size, sizeof(double));
    for(i=0; i<dim; ++i) {
    graph[i]=calloc(size, sizeof(double));
    }

    for(i=0; i<size; i++) {
      for(j=0; j<size; j++) {
	if(i==j) { graph[i][j]=0; }
	//create symmetric matrix
	else { graph[i][j]=graph[j][i]=(double)rand()/(double)RAND_MAX; }
      }
    }
    GRAPH=graph;
}


void update_distance(int node, int n)
{
    int i;
    for(i=0; i<n; i++) {
        if((GRAPH[node][i]!=0) && (d[i]>GRAPH[node][i]))
        {
                d[i]=GRAPH[node][i];
                used[i]=node;
		//heap_decrease_key(Q, i, GRAPH[node][i]);
		
        }
   }
}

//Prim's algorithm
double mst_prim(int n)
{
    int i, j, min=-1;
    double dist=0, total=0, count=0;
    /* memory allocation for the heap*/
    Q=heap_alloc(n);

    /*memory allocation for the symmetric matric with randomly distribured weights*/
    initialize_graph(n);

    /*inilization of starting values for the distances <- INFINITY and P-field which denotes the edge 
    if in_tree=0 the edge doesn't exist*/
    for(i=0; i<n; i++) { d[i]=INFINITY; in_tree[i]=0; }

    for(j=1; j<n; j++) {
	if(GRAPH[0][j]!=0) {
	  dist=d[j];
	  heap_insert(Q, j, dist);
	}
    }
    update_distance(0, n);
    in_tree[0]=1;

    i=1;

    for(j=1; j<n; j++)
    {
	if(in_tree[i]==0) {min=heap_extract_min(Q);}
	if((GRAPH[i][j]!=0) && (GRAPH[i][j]<d[j])) heap_decrease_key(Q, j, GRAPH[i][j]);


	
	in_tree[min]=1;
	total+=d[min];

	update_distance(min, n);
	i++;
    }

    heap_free(Q);

    return total;
}


int main(int argc, char* argv[])
{
    int n=20,n2=40, n3=60, n4=80, n5=100, i, j;
    double opt_dist[MAXSIZE], total20=0, average20, total40;
    double average40, total60, average60, total80, average80, average100, total100;
    int iter=100;

    /*computation of expected average values of weights for n=20, n=40, n=60, n=80, n=100 */
    for(i=0; i<iter; i++ )
    {
	opt_dist[i]=mst_prim(n);

	total20+=opt_dist[i];
    }

    average20=total20/iter;
    printf("expected average weight for-%d vertexes", n);
    printf("\n%1.2f\n", average20);

    for(i=0; i<iter; i++ )
    {
	opt_dist[i]=mst_prim(n2);

	total40+=opt_dist[i];
    }
    
    average40=total40/iter;
    printf("expected average weight for-%d vertexes", n2);
    printf("\n%1.2f\n", average40);

    for(i=0; i<iter; i++ )
    {
	opt_dist[i]=mst_prim(n3);

	total60+=opt_dist[i];
    }
    
    average60=total60/iter;
    printf("expected average weight for-%d vertexes", n3);
    printf("\n%1.2f\n", average60);

    for(i=0; i<iter; i++ )
    {
	opt_dist[i]=mst_prim(n4);

	total80+=opt_dist[i];
    }
    
    average80=total80/iter;
    printf("expected average weight for-%d vertexes", n4);
    printf("\n%1.2f\n", average80);

    for(i=0; i<iter; i++ )
    {
	opt_dist[i]=mst_prim(n5);

	total100+=opt_dist[i];
    }
    
    average100=total100/iter;
    printf("expected average weight for- %d vertexes", n5);
    printf("\n%1.2f\n", average100);

    free(GRAPH);
    return 0;
}






