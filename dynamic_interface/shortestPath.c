#include "graph.h"
#include "road.h"
#include "shortestPath.h"
#define INF INT_MAX

int verif(int *isMarked, int n){
    for(int i = 0; i<n; i++){
        if(isMarked[i] == -1){
            return -1;
        }
    }
    return 0;
}

int min(int n, Road matrix[][MAX_VERTICES], int i){
    int min = INF;
    int s = -1;
    for(int j = 0; j<n; j++){
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

void dijkstra(int n, Road matrix[][MAX_VERTICES], int start){
    int length[n];
    int node = start;
    length[node] = 0;
    int isMarked[n];
    int next[n];
    isMarked[node] = 0;
    int min;
    for(int i = 0; i<n; i++){
        if(i != node){
            length[i] = INF;
            isMarked[i] = -1;
        }
        next[i] = INF;
    }
    isMarked[node] = 0;
    min = INF;
    while(verif(isMarked, n) != 0){
        min = INF;
        isMarked[node] = 0;
        for(int j = 0; j<n; j++){
            if(matrix[node][j].weight != 0){
                if(matrix[node][j].weight + length[node] < length[j]){
                    length[j] = matrix[node][j].weight + length[node];
                    if(next[node] == INF || next[node] == start){
                        next[j] = node;
                    }
                    else{
                        next[j] = next[node];
                    }
                }
            }
        }
        for(int i = 0; i<n; i++){
            if(length[i] < min && isMarked[i] != 0){
                node = i;
                min = length[i];
            }
        }
        if(min == INF){
            for(int i = 0; i < n; i++){
                if(length[i] == INF){
                    length[i] = -1;
                    isMarked[i] = 0;
                }
            }
        }
    }
    for(int i = 0; i<n; i++){
        printf("%d ", length[i]);
    }
    printf("\n");
    for(int i = 0; i<n; i++){
        printf("%d ", next[i]);
    }
    printf("\n");
}

