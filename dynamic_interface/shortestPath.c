#include "graph.h"
#include "road.h"

int verif(int *isMarked, int num_vertices){
    for(int i = 0; i<num_vertices; i++){
        if(isMarked[i] == -1){
            return -1;
        }
    }
    return 0;
}

int min(int num_vertices, Road matrix[][MAX_VERTICES], int i){
    int min = INF;
    int s = -1;
    for(int j = 0; j<num_vertices; j++){
        // printf("%.f ", matrix[i][j].weight);
        if(matrix[i][j].weight != 0){
            printf("%.f ", matrix[i][j].weight);
            if(matrix[i][j].weight < min){
                min = matrix[i][j].weight;
                s = j;
            }
        }
    } 
    return s;
}

void dijkstra(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start) {
    int length[num_vertices];
    int node = start;
    length[node] = 0;
    int isMarked[num_vertices];
    int next[num_vertices];
    isMarked[node] = 0;

    for (int i = 0; i < num_vertices; i++) {
        if (i != node) {
            length[i] = INF;
            isMarked[i] = -1;
        }
        next[i] = INF;
        vertices[start].predecessor[i] = -1;  
    }

    isMarked[node] = 0;
    int min = INF;

    while (verif(isMarked, num_vertices) != 0) {
        min = INF;
        isMarked[node] = 0;
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[node][j].weight != 0) {
                if (matrix[node][j].weight + length[node] < length[j]) {
                    length[j] = matrix[node][j].weight + length[node];
                    if (next[node] == INF || next[node] == start) {
                        next[j] = node;
                    } else {
                        next[j] = next[node];
                    }
                    vertices[start].predecessor[j] = node;  
                }
            }
        }

        for (int i = 0; i < num_vertices; i++) {
            if (length[i] < min && isMarked[i] != 0) {
                node = i;
                min = length[i];
            }
        }
        if (min == INF) {
            for (int i = 0; i < num_vertices; i++) {
                if (length[i] == INF) {
                    length[i] = -1;
                    isMarked[i] = 0;
                }
            }
        }
    }

    for (int i = 0; i < num_vertices; i++) {
        vertices[start].nextVertex[i] = next[i];
        vertices[start].shortestPath[i] = length[i];
    }
}

void display_dijkstra(int num_vertices, Vertex vertices[], int start) {
    printf("Next vertices from node %d:\n", start);
    for (int i = 0; i < num_vertices; i++) {
        if (vertices[start].nextVertex[i] != INF)
            printf("%d ", vertices[start].nextVertex[i]);
        else
            printf("INF ");
    }
    printf("\n");

    printf("Shortest path lengths from node %d:\n", start);
    for (int i = 0; i < num_vertices; i++) {
        if (vertices[start].shortestPath[i] != INF)
            printf("%d ", vertices[start].shortestPath[i]);
        else
            printf("INF ");
    }
    printf("\n");
}

int build_path_points(int num_vertices, Vertex vertices[], int start, int target, Point* path_points, int* point_count) {
    int current = target;
    *point_count = 0; 

    while (current != start && current != -1) { // Parcours the predecessor's path
        path_points[*point_count] = (Point){ vertices[current].x, vertices[current].y };
        (*point_count)++;
        current = vertices[start].predecessor[current]; 
    }

    if (current == start) {
        path_points[*point_count] = (Point){ vertices[start].x, vertices[start].y };
        (*point_count)++;
    }

    return *point_count;
}