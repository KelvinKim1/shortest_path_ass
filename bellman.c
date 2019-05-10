#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct Edge{
    int src, dest, weight;
}Edge;

struct Graph
{
    // V-> Number of vertices, E-> Number of edges
    int V, E;
    struct Edge* edge;
}Graph;

struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = malloc(sizeof(Edge)*E);
    return graph;
}

int name_map(char* name){
    int a = (name[0]-'A')*26*26 + (name[1]-'A')*26 + (name[2]-'A');
    return a;
}
void free_graph(struct Graph * graph){
    free(graph->edge);
    free(graph);
}


int BellmanFord(struct Graph* graph, int src, int dest)
{
    int V = graph->V;
    int E = graph->E;
    int dist[V];


    //Initialize distances from source to vertices
    for (int i = 0; i < V; i++){
        dist[i] = INT_MAX;
    }
    dist[src] = 0;

    for (int i = 1; i <= V-1; i++)
    {
        for (int j = 0; j < E; j++)
        {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]){
                dist[v] = dist[u] + weight;
            }
        }
    }
    return dist[dest];
}

int main(int argc, char **argv) {
    FILE *fp;
    FILE *fp2;
    char* airport = argv[1];
    char* dists = argv[2];
    int MAXCHAR = 1000;
    char str[MAXCHAR];
    char command[MAXCHAR];
    char* com;
    int quit = 0;
    while (quit == 0){
        printf("Enter Command> ");
        fgets(command,MAXCHAR,stdin);
        char* len = command;
        if (strlen(len) > 1){
            com = strtok(command," \n");
        } else {
            continue;
        }
        if (strcmp(com, "help") == 0){
            printf("quit: quits the program\nairports: prints all airports serviced by FTA\ndistance <airport1> <airport2> prints path from one airport to another, using only routes flown by the airline. Also prints the total length of the shortest path.");
        }
        if (strcmp(com, "quit") == 0){
            quit = 1;
        }
        if (strcmp(com, "airports") == 0){
            fp = fopen(airport, "r");
            if (fp == NULL){
                printf("Could not open file");
                return 1;
            }
            while (fgets(str, MAXCHAR, fp) != NULL){
                printf("%s",str);
            }
        }
        if (strcmp(com, "distance") == 0){
            char* airports[] = {"src","dest"};
            int k = 0;
            
            while(com != NULL){
                com = strtok(NULL, " \n");
                airports[k] = com;
                k++;
            }
            if (airports[0] == '\0' || airports[1] == '\0'){
                printf("Your airports names are invalid\n");
                continue;
            }

            char* src = airports[0];
            char* dest = airports[1];

            fp = fopen(airport, "r");
            fp2 = fopen(dists,"r");
            if (fp == NULL || fp2 == NULL){
                printf("Could not open file");
                return 1;
            }
            int edge = 0;
            while (fgets(str, MAXCHAR, fp2) != NULL){
                edge++;
            }
            int vertex = 0;
            char* name;
            int map[17576];
            int y = 0;

            while (fgets(str, MAXCHAR, fp) != NULL){
                name = strtok(str, " \t");
                map[name_map(name)] = vertex;
                if (strcmp(name,airports[0]) == 0){
                    y++;
                }
                if (strcmp(name,airports[1]) == 0){
                    y++;
                }
                vertex++;
            }
            if (y != 2){
                printf("Your airports names are invalid\n");
                continue;
            }

            struct Graph* graph = createGraph(vertex,edge);
            
            int i = 0;
            char* token;
            rewind(fp2);
            while (fgets(str, MAXCHAR, fp2) != NULL){
                token = strtok(str, " \t");
                graph->edge[i].src = map[name_map(token)];
                graph->edge[i+1].dest = map[name_map(token)];
                char* tokens[2];
                int j = 0;
                while(token != NULL){
                    token = strtok(NULL, " \t");
                    tokens[j] = token;
                    j++;
                }
                graph->edge[i].dest = map[name_map(tokens[0])];
                graph->edge[i+1].src = map[name_map(tokens[0])];
                graph->edge[i].weight = atoi(tokens[1]);
                graph->edge[i+1].weight = atoi(tokens[1]);
                i+=2;
            }
            int distance = BellmanFord(graph, map[name_map(src)], map[name_map(dest)]);
            printf("Total distance is %d\n", distance);
            
            fclose(fp);
            free_graph(graph);
        }
    }
    return 0;
}
