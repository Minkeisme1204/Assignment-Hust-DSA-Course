#include <stdio.h>
#include <stdlib.h>
#define MAXN 1024
typedef struct Graph {
    int nodes; 
    int **edges; 
    int *weights;
    int *visited; 
} Graph; 

Graph *createGraph() {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    return graph;
};

void enterGraph(Graph* graph) {
    printf("Enter the number of nodes: ");
    scanf("%d", &graph->nodes);
    int n; 
    printf("Enter the number of edges: ");
    scanf("%d", &n);

    graph->edges = (int**)malloc(graph->nodes * sizeof(int*));
    for (int i = 0; i < graph->nodes; i++) {
        graph->edges[i] = (int*)malloc(graph->nodes * sizeof(int));
        for (int j = 0; j < graph->nodes; j++) {
            graph->edges[i][j] = 0;
        }
    }

    graph->visited = (int*)malloc(graph->nodes * sizeof(int));

    printf("Enter edges: \n"); 
    for (int i = 0; i < n; i++) {
        int u, v; 
        scanf("%d", &u);
        scanf("%d", &v);

        graph->edges[u*graph->nodes + v] = 1; 
    }
}

void BFS(Graph* graph) {
    int queue[MAXN];
    int front, back; 
    front = back = -1;

    int start_node = 0;
    front = 0; 
    back ++; 
    queue[back] = start_node; 

    while (front <= back) {
        int node = queue[front]; 
        printf("%d ", node);
        graph->visited[node] = 1;
        front++;

        for (int i = 0; i < graph->nodes; i++) {
            if (graph->edges[node*graph->nodes + i] == 1 && graph->visited[i] == 0) {
                back++; 
                queue[back] = i; 
            }
        }
    }
    
}

int main() {
    Graph* graph = createGraph();
    enterGraph(graph);
    printf("DFS traversal: ");
    BFS(graph);
    return 0;
}