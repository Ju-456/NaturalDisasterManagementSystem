#include "graph.h"

int main() {
    Road matrix[100][100];

    Vertex vertices[100];
    int num_vertices = 0; // in the top of the txt

    int num_roads = 0;
    Road roads[100];

    char full_path[PATH_MAX];
    build_path(full_path);

    if (load_adjacency_matrix(matrix, full_path, &num_vertices)) {

        // Vertex's part
        init_vertex_characteristics(vertices, matrix, num_vertices);
        display_vertex_characteristics(vertices, num_vertices);
        
        // Road's part
        init_roads_characteristics(matrix, num_vertices);
        num_roads = count_roads(roads, matrix, num_vertices);
        display_roads_characteristics(vertices, roads, num_roads);

        printf("Road states matrix before the earthquake:\n");
        display_roads_state_matrix(matrix, num_vertices);

        // The earthquake
        earthquake (num_vertices, matrix);

        printf("Road states matrix after the earthquake:\n");
        display_roads_state_matrix(matrix, num_vertices);

    } else {
        printf("Error ! Failed to load the adjacency matrix from the file.\n");
        return 1;
    }

    return 0;
}
