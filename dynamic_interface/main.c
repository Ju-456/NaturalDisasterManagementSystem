#include "road.h"
#include "vertex.h"
#include "window.h"
#include "shortestPath.h"

int main() {
    Road matrix[100][100];

    Vertex vertices[100];
    int num_vertices = 0; // in the top of the txt
    int hospitals = 0, cities = 0, warehouses = 0;

    int num_roads = 0;
    Road roads[100];

    char full_path_txt[PATH_MAX] = {0};
    char full_path_json[PATH_MAX] = {0};
    
    build_path(full_path_txt, "MatAdj_txt/", FILENAME_TXT);
    build_path(full_path_json, "Graph_json/", FILENAME_JSON);
    
    if (load_adjacency_matrix(matrix, full_path_txt, &num_vertices)) {

        // General's part
        // display_adjacency_matrix(matrix, full_path_txt, &num_vertices);  // just a tool to check issue

        // Vertex's part
        init_vertex_characteristics(vertices, matrix, num_vertices);
        attribute_vertex_characteristics(vertices, num_vertices, &hospitals, &cities, &warehouses);

        // Road's part
        init_roads_characteristics(matrix, num_vertices);
        num_roads = count_roads(roads, matrix, num_vertices);

        // printf("Road states matrix before the earthquake:\n");
        // display_roads_state_matrix(matrix, num_vertices);

        // earthquake(num_vertices, matrix);

        // printf("Road states matrix after the earthquake:\n");
        // display_roads_state_matrix(matrix, num_vertices);
        dijkstra(num_vertices, matrix, 0);
        init_window_custom(full_path_json, num_vertices, vertices, roads, num_roads, matrix);

        //Display's part
        //display_roads_characteristics(vertices, roads, num_roads); //doesn't work before the last function
        //display_vertex_characteristics(vertices, num_vertices); //doesn't work before the last function
    } else {
        printf("Error ! Failed to load the adjacency matrix from the file.\n");
        return 1;
    }

    return 0;
}
