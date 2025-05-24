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

int verifP(int num_vertices, int roads[][MAX_VERTICES], int s){
    int compteur;
    for(int i = 0; i<num_vertices; i++){
        compteur = 0;
        for(int j =0; j<num_vertices; j++){
            if(roads[j][i] == 1){
                compteur++;
            }
        }
        if(compteur == 0){
            return -1;
        }
    }
    return 0;
}

int dfs(int current, int roads[][MAX_VERTICES], int visited[], int marked[], int num_vertices) {
    if(visited[current] == 1 || marked[current] == 1){
        return 1;
    }
    visited[current] = 1;
    marked[current] = 1;
    for(int i = 0; i<num_vertices; i++){
        if(roads[current][i] == 1){
            if(dfs(i, roads, visited, marked, num_vertices)){
                return 1;
            }
        }
    }
        marked[current] = 0;
    return 0;
}

void edmonds(Road matrix[][MAX_VERTICES], Vertex vertices[], int num_vertices, int roads[][MAX_VERTICES]) {
    int i, j, root = 0;  // Choix arbitraire de la racine, ici 0

    // Initialiser la matrice roads à 0
    for (i = 0; i < num_vertices; i++)
        for (j = 0; j < num_vertices; j++)
            roads[i][j] = 0;

    // Tableau pour stocker la meilleure arête entrante par sommet
    int min_from[MAX_VERTICES];
    int min_incoming[MAX_VERTICES];

    for (i = 0; i < num_vertices; i++) {
        min_incoming[i] = INF;
        min_from[i] = -1;
    }

    // Étape 1 : Trouver pour chaque sommet l'arête entrante minimale (sauf racine)
    for (j = 0; j < num_vertices; j++) {
        if (j == root) continue;
        for (i = 0; i < num_vertices; i++) {
            if (matrix[i][j].weight > 0 && matrix[i][j].weight < min_incoming[j]) {
                min_incoming[j] = matrix[i][j].weight;
                min_from[j] = i;
            }
        }
    }

    // Étape 2 : Construire l'arbre couvrant minimal orienté sans gestion des cycles (simplifié)
    // (Attention: ici pas de contraction de cycles)
    for (j = 0; j < num_vertices; j++) {
        if (j == root) continue;
        if (min_from[j] != -1) {
            roads[min_from[j]][j] = 1;
        }
    }
}