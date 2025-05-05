#include "graph.h"
#include "travel.h"

void init_city_need(int num_vertices,Road matrix[][MAX_VERTICES],Vertex vertices[MAX_VERTICES]){
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[i][j].state == 1 ||matrix[i][j].state == 2){
                vertices[i].need = 1; // in the end of the process, vertices[i].need == 1
            }
        }
    }
}

void init_type_of_issue(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES]){
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[i][j].state == 1 ||matrix[i][j].state == 2){
                vertices[i].issue = (rand() % 2) + 1; // 1 = hopital or 2 = warehousse
            }
        }
    }
}

void init_travel_time(int num_vertices, Road matrix[][MAX_VERTICES]) {
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[i][j].state == 1) {
                matrix[i][j].road_capacity = matrix[i][j].road_capacity * 0.8; // decrease of 20% 
                if (matrix[i][j].weight > 15){
                    matrix[i][j].travel_time = matrix[i][j].weight * 1.5;
                }
                else {
                    matrix[i][j].travel_time = matrix[i][j].weight * 3;
                }
            } else if (matrix[i][j].state == 2){
                matrix[i][j].road_capacity = matrix[i][j].road_capacity * 0.6; // decrease of 40% 
                if (matrix[i][j].weight > 15){
                    matrix[i][j].travel_time = matrix[i][j].weight * 2;
                }
                else {
                    matrix[i][j].travel_time = matrix[i][j].weight * 4;
                }
            }
        }
    }
}

void display_info_travel(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES]) {
    printf("=== City Information ===\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("City %d: need = %d, issue = %d\n", i, vertices[i].need, vertices[i].issue);
    }

    printf("\n=== Road Information ===\n");
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[i][j].state == 1 || matrix[i][j].state == 2) {
                int time = matrix[i][j].travel_time;
                int hours = time / 60;
                int minutes = time % 60;

                printf("Road from %d to %d: state = %d, capacity = %d, travel time = %dh%02d\n", 
                       i, j, matrix[i][j].state, matrix[i][j].road_capacity, hours, minutes);
            }
        }
    }
}