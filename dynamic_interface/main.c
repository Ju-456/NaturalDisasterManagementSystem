#include "road.h"
#include "vertex.h"
#include "window.h"
#include "travel.h"
#include "vertexGroup.h"

int main() {
    Road matrix[MAX_VERTICES][MAX_VERTICES];
    Road matrix_inverse[MAX_VERTICES][MAX_VERTICES];
    int result[MAX_VERTICES];
    int result_inverse[MAX_VERTICES];

    Vertex vertices[MAX_VERTICES];
    int num_vertices = 0; // in the top of the txt
    int hospitals = 0, cities = 0, warehouses = 0;
    int pos = 0;

    int num_roads = 0;
    Road roads[MAX_VERTICES];

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
        
        // VertexGroup's part
        depth_first_search_full(num_vertices, matrix);
        init_matrix_1d_zero(num_vertices, result);
        init_matrix_1d_zero(num_vertices, result_inverse);
        init_matrix_2d_zero(num_vertices, matrix_inverse);

        left_right_root_full(num_vertices, matrix, result, &pos);
        inverse_matrix_1d_full(num_vertices, result, result_inverse);
        inverse_matrix_2d_full(num_vertices, matrix, matrix_inverse);
        depth_first_search_inverse(num_vertices, matrix, matrix_inverse, result_inverse);
        read_matrix_2d_cfc(num_vertices, matrix);
        
        int order_for_intervention = 1;

        // printf("Road states matrix after the earthquake:\n");
        // display_roads_state_matrix(matrix, num_vertices);
        edmonds(matrix, vertices, num_vertices);
        init_window_custom(full_path_json, num_vertices, vertices, roads, num_roads, matrix, order_for_intervention);
        
        init_roads_id(vertices, roads, num_roads);
        
        //Display's part
        // display_roads_characteristics(vertices, roads, num_roads); //doesn't work before the last function
        // display_vertex_characteristics(vertices, num_vertices); //doesn't work before the last function       

    } else {
        printf("Error ! Failed to load the adjacency matrix from the file.\n");
        return 1;
    }

    return 0;
}
