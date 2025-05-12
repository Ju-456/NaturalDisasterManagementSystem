#include "graph.h"

int earthquake(int num_vertices, Road matrix[][MAX_VERTICES], int num_roads) {
    srand(time(NULL)); 
    printf("Be careful !!! EARTHQUAKE : ");

    int localisation_NS = rand() % 2; // North = 0, South = 1
    int localisation_EW = rand() % 2; // East = 0, West = 1

    int middle = num_vertices / 2;
    int affected = 0;
    int min_affected_roads = num_roads * 0.3;
    printf("min_affected_roads = %d",min_affected_roads);

    if (localisation_NS == 0 && localisation_EW == 0) {
        printf("The earthquake will affect more the North-West of the map..\n");
        for (int i = 0; i <= middle; i++) {
            for (int j = 0; j <= middle; j++) {
                if (affected >= min_affected_roads) return affected;
                if (matrix[i][j].weight > 0) {
                    matrix[i][j].state = (rand() % 2) + 1;
                    affected++;
                }
            }
        }
    } else if (localisation_NS == 0 && localisation_EW == 1) {
        printf("The earthquake will affect more the North-East of the map..\n");
        for (int i = 0; i <= middle; i++) {
            for (int j = middle; j < num_vertices; j++) {
                if (affected >= min_affected_roads) return affected;
                if (matrix[i][j].weight > 0) {
                    matrix[i][j].state = (rand() % 2) + 1;
                    affected++;
                }
            }
        }
    } else if (localisation_NS == 1 && localisation_EW == 0) {
        printf("The earthquake will affect more the South-West of the map..\n");
        for (int i = middle; i < num_vertices; i++) {
            for (int j = 0; j <= middle; j++) {
                if (affected >= min_affected_roads) return affected;
                if (matrix[i][j].weight > 0) {
                    matrix[i][j].state = (rand() % 2) + 1;
                    affected++;
                }
            }
        }
    } else {
        printf("The earthquake will affect more the South-East of the map..\n");
        for (int i = middle; i < num_vertices; i++) {
            for (int j = middle; j < num_vertices; j++) {
                if (affected >= min_affected_roads) return affected;
                if (matrix[i][j].weight > 0) {
                    matrix[i][j].state = (rand() % 2) + 1;
                    affected++;
                }
            }
        }
    }

    return affected;
}
