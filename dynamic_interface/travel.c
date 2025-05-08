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

int research_closest_vertex(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start, int target_type) {
    dijkstra(num_vertices, matrix, vertices, start);

    int min_distance = INF;
    int closest_vertex = -1;

    for (int i = 0; i < num_vertices; i++) {
        if (vertices[i].type == target_type && vertices[start].shortestPath[i] != -1 && vertices[start].shortestPath[i] < min_distance) {
            min_distance = vertices[start].shortestPath[i];
            closest_vertex = i;
        }
    }

    return closest_vertex;
}

void display_research_closest_vertex(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start, int target_type) {
    for(int i = 0; i<num_vertices; i++){
        // int closest_vertex = research_closest_vertex(num_vertices, matrix, vertices, i, need);
        int need = travel_to_city(num_vertices, matrix, vertices, i);
        int closest_vertex = research_closest_vertex(num_vertices, matrix, vertices, i, need);
        if (closest_vertex != -1 && need != -1) {
            int distance = vertices[i].shortestPath[closest_vertex];
            printf("From city %d, closest type %d is city %d (distance: %d)\n", 
                i,need, closest_vertex, distance);
        }
        else if(need == -1){
            printf("this city hasn't been touched by the earthquake\n");
        } 
        else {
            printf("No reachable vertex of type %d from city %d\n", need, i);
        }
    }
}

int travel_to_city(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES], int start) {
    // for (int i = 0; i < num_vertices; i++) {
        if (vertices[start].need == 1) {
            if (vertices[start].issue == 1) {
                // research_closest_vertex(num_vertices, matrix, vertices, i, 1); // need hospital ressources
                return 1;
            } else if (vertices[start].issue == 2) {
                // research_closest_vertex(num_vertices, matrix, vertices, i, 2); // need warehousse ressources
                return 2;
            }
        }
    // }
    return -1;
}