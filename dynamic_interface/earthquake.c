#include "graph.h"

int earthquake (int num_vertices, Road matrix[][MAX_VERTICES]){
    srand(time(NULL)); 
    printf("Be careful !!! EARTHQUAKE : ");

    int localisation_NS = rand() % 2; // North = 0, South = 1
    int localisation_EW = rand() % 2; // East = 0, West = 1

    int middle_of_matrix = num_vertices / 2;

    if (localisation_NS == 0 && localisation_EW == 0) {
        printf("The earthquake will affect more the North-West of the map.. \n");
        for (int i = 0; i <= middle_of_matrix; i++) {
            for (int j = 0; j <= middle_of_matrix; j++) {
                if (matrix[i][j].weight > 0) {
                    matrix[i][j].state = (rand() % 2) + 1; // 1 or 2
                } else {
                    matrix[i][j].state = 0;
                }
            }
        }
    } else if (localisation_NS == 0 && localisation_EW == 1) {
        printf("The earthquake will affect more the North-East of the map.. \n");
        for (int i = 0; i <= middle_of_matrix; i++) {
            for (int j = middle_of_matrix; j < num_vertices; j++) {
                if (matrix[i][j].weight > 0) {
                    matrix[i][j].state = (rand() % 2) + 1; // 1 or 2
                } else {
                    matrix[i][j].state = 0;
                }
            }
        }
    } else if (localisation_NS == 1 && localisation_EW == 0) {
        printf("The earthquake will affect more the South-West of the map.. \n");
        for (int i = middle_of_matrix; i < num_vertices; i++) {
            for (int j = 0; j <= middle_of_matrix; j++) {
                if (matrix[i][j].weight > 0) {
                    matrix[i][j].state = (rand() % 2) + 1; // 1 or 2
                } else {
                    matrix[i][j].state = 0;
                }
            }
        }
    } else {
        printf("The earthquake will affect more the South-East of the map.. \n");
        for (int i = middle_of_matrix; i < num_vertices; i++) {
            for (int j = middle_of_matrix; j < num_vertices; j++) {
                if (matrix[i][j].weight > 0) {
                    matrix[i][j].state = (rand() % 2) + 1; // 1 or 2
                } else {
                    matrix[i][j].state = 0;
                }
            }
        }
    }
    return 1;
}
