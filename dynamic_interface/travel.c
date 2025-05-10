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
            } 
            else if (matrix[i][j].state == 2){
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
        if ((vertices[i].need = 1) && (vertices[i].issue != 0)){
            printf("City %d: need = %d, issue = %d\n", i, vertices[i].need, vertices[i].issue);
        }
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
    printf("\n\n");
}

int research_closest_vertex(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start, int target_type) {
    dijkstra(num_vertices, matrix, vertices, start);
    display_dijkstra(num_vertices, vertices, start);
    int min_distance = INF;
    int closest_vertex = -1;

    for (int i = 0; i < num_vertices; i++) {
        if (vertices[i].type == target_type && vertices[start].shortestPath[i] != -1 && vertices[start].shortestPath[i] < min_distance) {
            min_distance = vertices[start].shortestPath[i];
            closest_vertex = i;
        }
    }
    vertices[start].closest = closest_vertex; 
    return closest_vertex;
}

void display_research_closest_vertex(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start, int target_type) {
    int closest_vertex = research_closest_vertex(num_vertices, matrix, vertices, start, target_type);

    if (closest_vertex != -1) {
        int distance = vertices[start].shortestPath[closest_vertex];
        const char* target_name = (target_type == 1) ? "hospital" : "warehouse";
        printf("From city %d (%s), closest %s is city %d (%s) (distance: %d)\n\n", 
               start, vertices[start].id,target_name, closest_vertex, vertices[closest_vertex].id, distance);
    } else {
        const char* target_name = (target_type == 1) ? "hospital" : "warehouse";
        printf("No reachable %s from city %d (%s)\n\n", target_name, start, vertices[start].id);
    }
}

void travel_to_city(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES], int* order_for_intervention) {
    for (int i = 0; i < num_vertices; i++) {
        if (vertices[i].need == 1) {// if you won't display, just call 'research_closest_vertex' only
            if (vertices[i].issue == 1) { 
                // research_closest_vertex(num_vertices, matrix, vertices, i, 1); // need hospital ressources
                display_research_closest_vertex(num_vertices, matrix, vertices, i, 1);
                display_priority_of_processing_vertices(num_vertices, matrix, vertices, i, 1, order_for_intervention);
            } else if (vertices[i].issue == 2) {
                // research_closest_vertex(num_vertices, matrix, vertices, i, 2); // need warehousse ressources
                display_research_closest_vertex(num_vertices, matrix, vertices, i, 2);
                display_priority_of_processing_vertices(num_vertices, matrix, vertices, i, 2, order_for_intervention);
            }
        }
    }
}

void display_priority_of_processing_vertices(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start, int target_type, int* order_for_intervention){
    int closest_vertex = research_closest_vertex(num_vertices, matrix, vertices, start, target_type);
    if (closest_vertex != -1) {
        int distance = vertices[start].shortestPath[closest_vertex];

        if (distance == 0){
            printf("This intervention for the city %d (%s) does not require travel (distance = 0).\nThe treatment of this issue is done at the same place.\n\n",start, vertices[start].id);
            (*order_for_intervention)++;
        } else {
            printf("This intervention for the city %d (%s) will be treated at priority %d because the distance is %d\n\n",start, vertices[start].id, *order_for_intervention, distance);
            (*order_for_intervention)++;
        }
    } else {
        printf("No reachable vertex of type %d from city %d (%s), We can't assign priority.\n\n",
               target_type, start, vertices[start].id);
    }
}

// to udpate status to graphic part
void update_closest_vertices(int num_vertices, Vertex vertices[]) {
    for (int i = 0; i < num_vertices; i++) {
        if (vertices[i].need != 1) {
            vertices[i].closest = -1; // Pas besoin => aucun sommet cible
            continue;
        }

        int min_distance = INT_MAX;
        int closest_index = -1;

        for (int j = 0; j < num_vertices; j++) {
            if (i == j) continue;
            if (vertices[j].issue != 0) continue; // destination déjà en cours de traitement
            if (vertices[i].shortestPath[j] <= 0) continue; // aucune route valide

            if (vertices[i].shortestPath[j] < min_distance) {
                min_distance = vertices[i].shortestPath[j];
                closest_index = j;
            }
        }

        vertices[i].closest = closest_index;
    }
}