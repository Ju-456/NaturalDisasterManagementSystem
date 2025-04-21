#ifndef ROAD
#define ROAD

#include "graph.h"

int count_roads(Road roads[], Road matrix[][100], int num_vertices) {
    int index = 0;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[i][j].weight > 0) {
                roads[index] = matrix[i][j];
                roads[index].start = i;
                roads[index].end = j;
                index++;
            }
        }
    }
    return index;
}

void init_roads_characteristics(Road matrix[][100], int num_vertices) {
    srand(time(NULL)); 

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[i][j].weight > 0) {
                matrix[i][j].road_capacity = matrix[i][j].weight / 2; 
                matrix[i][j].state = (rand() % 2) + 2; // 2 or 3
            } else {
                matrix[i][j].road_capacity = 0;
                matrix[i][j].state = 0;
            }
        }
    }
}

int display_roads_state_matrix(Road matrix[][100], int num_vertices) {
    //printf("Road states matrix:\n");
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            printf("%d ", matrix[i][j].state);
        }
        printf("\n");
    }
    return 0;
}

char *get_type_name(int type) {
    switch(type) {
        case 0: return "city";
        case 1: return "hospital";
        case 2: return "warehouse";
        default: return "unknown";  // In case of error
    }
}

void display_roads_characteristics(Vertex vertices[], Road roads[], int num_roads) {
    printf("There are %d roads in this map:\n\n", num_roads);
    for (int k = 0; k < num_roads; k++) {
        int i = roads[k].start;
        int j = roads[k].end;

        printf("Road %d: %s -> %s\n", k + 1, vertices[i].id, vertices[j].id); // to skip special character
        printf("Type: (%s -> %s)\n", get_type_name(vertices[i].type), get_type_name(vertices[j].type));
        printf("State: %d\n", roads[k].state);
        printf("Weight: %.0f\n", roads[k].weight);
        printf("Road capacity: %d\n", roads[k].road_capacity);
        printf("----------------------------------\n");
    }
}

#endif
