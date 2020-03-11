#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1
#define MAX_VERTEX 100
#define MALLOC(p, s) \
    if(!((p) = malloc(s))) {\
        fprintf(stderr, "Insufficient memory");\
        exit(EXIT_FAILURE);\
    }
#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))

typedef struct node *nodePointer;
typedef struct node{
    union{
        short int visited;
        int weight;
    } u;
    int vertexno;
    nodePointer link;
};
struct inputedge{
    int node1;
    int node2;
    int weight;
};
nodePointer hdnode[MAX_VERTEX];

nodePointer newNode()
{
    nodePointer temp;
    MALLOC(temp, sizeof(*temp));
    return temp;
}

void addEdge(int first, int second, int w)
{
    nodePointer p = hdnode[first];
    nodePointer q = hdnode[second];
    nodePointer t1, t2;
    MALLOC(t1, sizeof(*t1));
    MALLOC(t2, sizeof(*t2));
    while(p->link) p = p->link;
    p->link = t1;
    t1->link = NULL;
    t1->vertexno = second;
    t1->u.weight = w;
    while(q->link) q = q->link;
    q->link = t2;
    t2->link = NULL;
    t2->vertexno = first;
    t2->u.weight = w;
}

void dfs(int v)
{
    nodePointer w;
    hdnode[v]->u.visited = TRUE;
    for(w = hdnode[v]; w; w = w->link)
    {
        if(!(hdnode[w->vertexno]->u.visited)) dfs(w->vertexno);
    }
}

void dfs_print(int v)
{
    nodePointer w;
    hdnode[v]->u.visited = TRUE;
    for(w = hdnode[v]; w; w = w->link)
    {
        if(!(hdnode[w->vertexno]->u.visited))
        {
            printf(" >> %d %d %d\n", v, w->vertexno, w->u.weight);
            dfs_print(w->vertexno);
        }
    }
}

void main()
{
    int numvertex, numedge, i, j, min, t;
    int addededge;                     // number of edges added to the resulting tree
    int leftedge;                      // number of edges left waiting to be added to the tree
    struct inputedge temp;             // used for sorting input edges in nondecreasing order
    printf(" __________________________________________________________________________________\n\n\n");
    printf("                       COSE213 Assignment#3 - Kruskal's Algorithm -\n\n");
    printf(" __________________________________________________________________________________\n");
    printf("  - 2016320213 김형민 - \n\n");
    printf(" Enter the number of vertices : ");
    do
    {
        scanf("%d", &numvertex);
        if(numvertex > MAX_VERTEX) printf(" The number of vertices cannot be larger than %d.\n   Please enter again : ", MAX_VERTEX);
    }while(numvertex > MAX_VERTEX);
    short int leftout_vertex[numvertex];
    for(i = 0; i < numvertex; i++)
    {
        hdnode[i] = newNode();
        hdnode[i]->u.visited = FALSE;
        hdnode[i]->vertexno = i;
        hdnode[i]->link = NULL;
        leftout_vertex[i] = FALSE;
    }
    printf(" Input the number of edges in the graph : ");
    do
    {
        scanf("%d", &numedge);
        if(numedge < (numvertex - 1)) printf(" The number of edges cannot be smaller than %d.\n   Please enter again : ", numvertex - 1);
        else if(numedge > (numvertex*(numvertex - 1)/2)) printf(" The number of edges is larger than the max possible edges.\n   Please enter again : ");
    }while(numedge < (numvertex - 1) || numedge > (numvertex*(numvertex - 1)/2));
    struct inputedge edge[numedge];
    printf(" Input the edges with vertices and the weight divided by spaces : \n");
    for(i = 0; i < numedge; i++)
    {
        printf(" ");
        scanf(" %d%d%d", &edge[i].node1, &edge[i].node2, &edge[i].weight);
        if(edge[i].node1 == edge[i].node2)
        {
            printf(" Self Edges are now allowed. Please enter again : \n");         // Restriction 1
            i--;
        }
        else if(edge[i].weight <= 0)
        {
            printf(" The weight cannot be less than or equal to 0. Please enter again : \n");           // Restriction 2
            i--;
        }
        else if(edge[i].node1 >= numvertex || edge[i].node2 >= numvertex)
        {
            printf(" The vertex number cannot be greater than the number of vertices. Please enter again : \n");            // Restriction 3
            i--;
        }
        else
        {
            for(j = 0; j < i; j++)
            {
                if(((edge[i].node1 == edge[j].node1) && (edge[i].node2 == edge[j].node2)) || ((edge[i].node1 == edge[j].node2) && (edge[i].node2 == edge[j].node1)))
                {
                    printf(" Overlapping Edges are not allowed. Please enter again : \n");          // Restriction 4
                    i--;
                }
            }
        }
    }
    for(i = 0; i < numedge - 1; i++)
    {
        min = i;
        for(j = i + 1; j < numedge; j++)
        {
            if(edge[j].weight < edge[min].weight)
                min = j;
        }
        SWAP(edge[i], edge[min], temp);
    }
    for(i = 0; i < numedge; i++)
    {
        if(edge[i].node1 > edge[i].node2) SWAP(edge[i].node1, edge[i].node2, t);
        leftout_vertex[edge[i].node1] = TRUE;
        leftout_vertex[edge[i].node2] = TRUE;
    }
    for(i = 0; i < numvertex; i++)
    {
        if(!leftout_vertex[i])
        {
            fprintf(stderr, " A node is singly left out with no incident edges. A minimum cost spanning tree cannot be created. \n");       //Restriction 5
            exit(EXIT_FAILURE);
        }
    }
    addEdge(edge[0].node1, edge[0].node2, edge[0].weight);      // Initially add the first edge to the tree
    addededge = 1;                           // Currently a single edge is added to the tree
    leftedge = numedge - 1;                  // One edge has been deleted from the input edges
    i = 1;
    while((addededge < (numvertex - 1)) && (leftedge > 0))
    {
        dfs(edge[i].node1);
        if(!hdnode[edge[i].node2]->u.visited)
        {
            addEdge(edge[i].node1, edge[i].node2, edge[i].weight);
            addededge++;
            leftedge--;
        }
        else edge[i].weight = 0;
        for(j = 0; j < numvertex; j++)
            hdnode[j]->u.visited = FALSE;
        i++;
    }
    if(addededge < (numvertex - 1))
    {
        fprintf(stderr, " There exists no spanning tree.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf(" The minimum cost spanning tree is : \n");
        j = 0;
        for(j = 0; j < numvertex; j++)
                hdnode[j]->u.visited = FALSE;
        dfs_print(0);
        exit(EXIT_SUCCESS);
    }
}
