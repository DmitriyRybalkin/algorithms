
#include <stdio.h>
#include <stdlib.h>

#define MinData (-32767)
#define INFINITY 2
#define MAXSIZE 100


struct HeapStruct {
    int Capacity;
    int Size;
    double *Elements;
};

double **GRAPH;
double w[MAXSIZE][MAXSIZE];


typedef struct HeapStruct *PriorityQueue;

PriorityQueue Q;

double IsEmpty(PriorityQueue H) {
    return H->Size == 0;
}

double IsFull(PriorityQueue H) {
    return H->Size == H->Capacity;
}

PriorityQueue InitializeQ(int MaxElements) {
    PriorityQueue H;


     H = malloc(sizeof ( struct HeapStruct));
     if (H == NULL)
         printf("Out of space!!!");

    /* Allocate the array plus one extra for sentinel */
    H->Elements = malloc((MaxElements + 1)*sizeof (double));
    if (H->Elements == NULL)
    printf("Out of space!!!");

    H->Capacity = MaxElements;
    H->Size = 0;

    return H;
}



void MakeEmpty(PriorityQueue H) {
    H->Size = 0;
}



void Insert(double X, PriorityQueue H) {
    int i;

    if (IsFull(H)) {
        printf("Priority queue is full");
        return;
    }

    for (i = ++H->Size; H->Elements[ i / 2 ] > X; i /= 2)
        H->Elements[ i ] = H->Elements[ i / 2 ];
    H->Elements[ i ] = X;
}


double ExtractMin(PriorityQueue H) {
    int i, Child;
    double MinElement, LastElement, temp;

     if (IsEmpty(H)) {
        printf("Priority queue is empty");
        return H->Elements[ 0 ];
    }
     MinElement = H->Elements[ 1 ];
     LastElement = H->Elements[ H->Size-- ];
    /* MIN-heapify */
    for (i = 1; i * 2 <= H->Size; i = Child) {
        /* Find smaller child */
         Child = i * 2;
         if (Child != H->Size && H->Elements[ Child + 1 ]
                < H->Elements[ Child ])
             Child++;

        /* Percolate one level */
         if (LastElement > H->Elements[ Child ]) {
            temp=H->Elements[ i ];
            H->Elements[ i ]=H->Elements[ Child ];
            H->Elements[ Child ]=temp; }
        else
             break;
    }
    H->Elements[ i ] = LastElement;
     return MinElement;
}

double FindMin(PriorityQueue H) {
    if (!IsEmpty(H))
        return H->Elements[ 1 ];
    printf("Priority Queue is Empty");
    return H->Elements[ 0 ];
}


void Destroy(PriorityQueue H) {
    free(H->Elements);
    free(H);
}


void InitializeGraph(int dim)
{
  int size=dim;
  int i, j;
  /* allocation of graph matrix */
  double **graph=(double**)calloc(size, sizeof(double));
  for(i=0; i<dim; ++i) {
   graph[i]=calloc(size, sizeof(double));
  }

  for(i=0; i<size; i++) {
    for(j=0; j<size; j++) {
      if(i==j) { graph[i][j]=0; }
      /* create symmetric matrix */
      else { graph[i][j]=graph[j][i]=(double)rand()/(double)RAND_MAX; }
    }
  }
  GRAPH=graph;
}
/* to neglet equal values of index symmetric permutations, take only up-triangular matrix 
 * - above main zero diagonal and assign it to global var w[i][j] */
void InitializeWeights(int dim)
{
  int i, j=0;
  int row=0, col=0; 
  for(i=0; i<dim; i++)
  {
    for(j=0; j<dim; j++)
    {
      w[i][j]=0;
    }
  }
  while(row!=dim && col!=dim)
   {
     if(GRAPH[row][col]==0) {
       for(j=col; j<dim; j++)
       {
	  w[row][j+1]=GRAPH[row][j+1];
       }
     }
     ++row;
     ++col;
   }
}

void FillQueue(int dim)
{
  int i, j, count=0;
  Q=InitializeQ(dim+1);

  for(i=0; i<dim+1; i++)
    Insert(2,Q);

}

void Prim(int dim)
{
  int size=dim;
  int i, j, row=0;
  double *u=(double*)calloc(size, sizeof(double));

  InitializeGraph(dim);

  for(i=0; i<dim; i++)
  {
    for(j=0; j<dim; j++)
    {
    printf("%1.2f\n", GRAPH[i][j]);
    }
  }
  InitializeWeights(dim);
  FillQueue(dim);

  for(i=0; i<dim+1; i++)
    printf("%1.2f\n", Q->Elements[i]);

  u[0]=ExtractMin(Q);
  u[1]=ExtractMin(Q);
  u[2]=ExtractMin(Q);
  u[3]=ExtractMin(Q);
  u[4]=ExtractMin(Q);
  printf("\n");
  for(i=0; i<dim; i++)
    printf("%1.2f\n", u[i]);
  free(u);
  
}

int main(int argc, char *argv[])
{
  int dim=5;
  Prim(dim);
  printf("display weights\n");


  free(GRAPH);
  Destroy(Q);
  
  return 0;
}
