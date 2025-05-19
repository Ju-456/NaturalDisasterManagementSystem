#include "graph.h"

// Vérification de la source unique (s) dans le graphe
int verifS(int n, Road matrix[][MAX_VERTICES]){
    int has_incoming_edge = 0;
    int multiple_sources_detected = 0;
    int s = 0;

    for(int i = 0; i < n; i++){
        has_incoming_edge = 0;
        for(int j = 0; j < n; j++){
            if(matrix[j][i].weight != 0){
                has_incoming_edge = 1;
            }
        }
        if(!has_incoming_edge){
            if(multiple_sources_detected){
                printf("Erreur : Plusieurs sources détectées.\n");
                return -1;
            }
            s = i;
            multiple_sources_detected = 1;
        }
    } 
    return s;
}

// Implémentation de l'algorithme d'Edmonds
void edmonds(Road matrix[][MAX_VERTICES], Vertex vertices[], int num_vertices){
    int s = verifS(num_vertices, matrix);
    if(s == -1){
        printf("Il n'existe pas de chemin vers tous les sommets dans le graphe.\n");
        return;
    }

    int isMarked[num_vertices];
    int roads[num_vertices][num_vertices];
    int min = INF;
    int k = -1;
    for(int i = 0; i<num_vertices; i++){
        dijkstra(num_vertices, matrix, vertices, i);
        isMarked[i] = 0;
    }
    for(int i = 0; i<num_vertices; i++){
        for(int j = 0; j<num_vertices; j++){
            roads[i][j] = 0;
        }
    }
    for(int i = 0; i<num_vertices; i++){
        for(int j = 0; j<num_vertices; j++){
            if(vertices[i].nextVertex[j] == i && vertices[i].shortestPath[j] < min && isMarked[j] != 1){
                k = j;
                min = vertices[i].shortestPath[j];
            }
        }
        if(k != -1){
            roads[i][k] = 1;
            isMarked[k] = 1;
        }
        k = -1;
        min = INF;
    }
    int test = 0;
    for(int i = 0; i<num_vertices; i++){
        min = INF;
        k = -1;
        test = 0;
        for(int j = 0; j<num_vertices; j++){
            if(roads[j][i] == 1){
                test = 1;
            }
        }
        if(test == 0){
            for(int j = 0; j<num_vertices; j++){
                if(matrix[j][i].weight != 0 && matrix[j][i].weight < min){
                    min = matrix[j][i].weight;
                    k = j;
                }
            }
            roads[k][i] = 1;
        }
    }
    for(int i = 0; i<num_vertices; i++){
        for(int j = 0; j<num_vertices; j++){
            if(roads[j][i] == 1){
                printf("road from %d to %d\n", j, i);
            }
        }
    }
}