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

void DFS(Graph* graph) {
    int stack[MAXN]; 
    int sp = -1; 
    
    int start_node = 0; 
    sp++;
    stack[sp] = start_node;; 
    
    while (sp >= 0) {
        int node = stack[sp]; 
        if (graph->visited[node] == 0) {
            graph->visited[node] = 1; 
            printf("%d ", node);
            for (int i = 0; i < graph->nodes; i++) {
                if (graph->edges[node*graph->nodes + i] == 1 && graph->visited[i] == 0) {
                    stack[sp + 1] = i; 
                    sp++; 
                    break; 
                }
            }
            if (sp == -1) {
                break; 
            }
        }
        sp--;
    }
}

int main() {
    Graph* graph = createGraph();
    enterGraph(graph);
    printf("DFS traversal: ");
    DFS(graph);
    return 0;
}