#include<stdio.h>
#include<stdlib.h>

//defination of adcjaceny list
typedef struct node
{
	int vertex;
	struct node *next;
}NODE;

//defination of graph
typedef struct graph
{
	int numVertices;
	int *indegree;
	NODE **adjList;
}GRAPH;

//create vertex
NODE *createNode(int V)
{
	NODE *newNode;
	newNode =(NODE*)malloc(sizeof(NODE));	
	if(newNode == NULL)
	{
		printf("Allocation Error\n");
		exit(0);
	}
	newNode->vertex = V;
	newNode->next = NULL;
	return newNode;
}

// create graph
GRAPH *createGraph(int numV)
{
	int i;
	GRAPH *graph = (GRAPH*)malloc(sizeof(GRAPH));
	if(graph == NULL)
	{
		printf("Allocation error.\n");
		exit(0);
	}
	graph->numVertices = numV;
	graph->adjList = (NODE**)malloc(numV * sizeof(NODE*));
	graph->indegree = (int*)calloc(numV, sizeof(int));
	for(i = 0; i < numV; i++)
		graph->adjList[i] = NULL;
	return graph;
	
}

void addEdge(GRAPH *graph, int src, int dest)
{
	// create a new node for the destination vertex
	NODE *newNode = createNode(dest);
	
	// insert the new node at the beginning of the adjacency list for the source vertex
	newNode->next = graph->adjList[src];
	graph->adjList[src] = newNode;
}

void printGraph(GRAPH *graph)
{
    int i;
    NODE *tmp;

    printf("\nAdjacency List:\n");
    for (i = 0; i < graph->numVertices; i++)
    {
        tmp = graph->adjList[i];
        printf("Course %d: ", i);

        while (tmp)
        {
            printf("%d -> ", tmp->vertex);
            tmp = tmp->next;
        }
        printf("NULL\n");
    }
    
    printf("Indegrees:\n");
    printf("Node\tIndegree\n");
    for(i = 0; i < graph->numVertices; i++)
    	printf("%d\t%d\n", i, graph->indegree[i]);
	printf("\n");
}

//convert adjacency matrix to adjacency list 
GRAPH *matrixToList(GRAPH *g, int **adjMtrx)
{
	int i, j;
	for(i = 0; i < g->numVertices; i++)
	{
		for(j = 0; j < g->numVertices; j++)
		{
			if(adjMtrx[i][j] == 1) // destination(j) : prerequsite course for source(i) course
				addEdge(g, i, j);
		}
	}
	return g;
}

void indegree(GRAPH *g)
{
	int i;
	NODE* current;
	for(i = 0; i < g->numVertices; i++)
	{
		if(g->indegree[i] != -1)
		{
			current = g->adjList[i];
			g->indegree[i] = 0;
			while(current)
			{
				g->indegree[i]++;
				current = current->next;
			}
		}
	}
}

void deleteNode(GRAPH* graph, int vertex) {
    if (vertex >= graph->numVertices) {
        printf("Node does not exist in the graph.\n");
        return;
    }
    
    // Remove all references to the node from other nodes in the list
    int i;
    for (i = 0; i < graph->numVertices; i++) {
        NODE* current = graph->adjList[i];
        NODE* prev = NULL;
        int stop = 1;
        while (stop && current) {
            if (current->vertex == vertex) 
			{
                if (prev != NULL) // if node is in the middle
                    prev->next = current->next;
            
				else // if node is the head
                    graph->adjList[i] = current->next; 
                
                free(current);
                stop = 0;
            }
            prev = current;
            current = current->next;
        }
    }
}

int check(GRAPH *g)
{
	int i = 0, stop = 1;
	while(stop && i < g->numVertices)
	{
		if(g->indegree[i] != -1)
			stop = 0;
		i++;
	}
	// 1 for all courses are taken, 0 for not taken
	return stop;
}

int availableCourses(GRAPH *g, int **semester)
{
	int i, k = 0, semest = 1, count;

	while(check(g) == 0)
	{
		count = 0;	

		for(i = 0; i < g->numVertices; i++)
		{
			if(g->indegree[i] == 0)
			{
				g->indegree[i] = -1;
				deleteNode(g, i);
				k = 0;
				while(k < g->numVertices && semester[1][k] != 0)
					k++;
				semester[0][k] = i;
				semester[1][k] = semest;
				count = 1;
			}
		}
		// If a loop becomes the algorithm must stop
		if(count == 0)
		{
			printf("This course list hasn't got any course without preconditions to start in some point\n");
			return 0;
		}
		semest++;
		indegree(g);
		//printGraph(g);
	}
	return 1;
}


void results(int **semester, int **adjMatrix, int N)
{
	int i, isavailable;
	//create a graph
	GRAPH *graph = createGraph(N);
	//convert adjacency matrix to adjacency list 
	graph = matrixToList(graph, adjMatrix);
	//calculate indegrees
    indegree(graph);
	printGraph(graph);
		
	//find the course path
	isavailable = availableCourses(graph, semester);

	if(isavailable)
	{	
		//print the result matrix
		for(i = 0; i < graph->numVertices; i++)
		{
			if(i != 0 && semester[1][i] != semester[1][i - 1])
			{
				printf("\n");
				printf(" Semester: %d -> Course%d", semester[1][i], semester[0][i]);
			}
			else if(i == 0)
				printf(" Semester: %d -> Course%d", semester[1][i], semester[0][i]);
			else if(semester[1][i] == semester[1][i - 1])
				printf("   Course%d    ", semester[0][i]);	
		}
	}
	
}


int main()
{
	int i, j, N, E, flag = 1;
	int **semester, **adjMatrix;
	
	while(flag)
	{
		printf("\nContinue: 1 / Exit 0\n");
		scanf("%d", &flag);
		if(flag)
		{
			printf("Enter the number of courses and prerequisite courses respectively:\n");
			scanf("%d %d", &N, &E);
		
			// create a result matrix -> first row is the course number second row is the representation of semester 
			semester = (int**)calloc(2, sizeof(int*));
			for(i = 0; i < 2; i++)
				semester[i] = (int*)calloc(N, sizeof(int));
				
			// create an adjacency matrix
			adjMatrix = (int**)calloc(N, sizeof(int*));
			for(i = 0; i < N; i++)
				adjMatrix[i] = (int*)calloc(N, sizeof(int));
				
			//Get adjacency matrix from user
			i = 0;
			printf("If courses are dependent enter 1 else enter 0\n");
			while(i < N && E != 0 )
			{
				j = 0;
				while(j < N && E != 0)
				{
					if(i != j)
					{
						printf("Is Course%d prerequisite for Course%d ?\n", j, i);
						scanf("%d", &adjMatrix[i][j]);
						if(adjMatrix[i][j] == 1)
							E--;
					}
					j++;
				}
				i++;
			}
			
			printf("\nAdjacency Matrix:\n");
			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
					printf("  %d  ", adjMatrix[i][j]);
				printf("\n");
			}
			
			results(semester, adjMatrix, N);
		}
	}
	return 0;
}

/*
EXAMPLE
correct
5
6
0
0
0
0
0
1
1
0
0
0
1
1
1
0
0
1

loop
6
12
0
0
1
0
0
1
1
0
0
0
1
1
1
1
0
0
1
1
0
1
0
0
1
0
1

correct
6
12
0
0
0
0
0
1
0
0
0
1
1
0
0
0
1
1
1
1
1
0
0
1
1
0
1
1

without  prerequisite course
3
0

*/
