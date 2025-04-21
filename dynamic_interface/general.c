#ifndef GENERAL
#define GENERAL

#include "graph.h"

// Used  for txt and json
void build_path(char *full_path, const char *base_path, const char *filename) {
    const char *home = getenv("HOME");
    if (home != NULL) {
        snprintf(full_path, PATH_MAX, "%s/NaturalDisasterManagementSystem/%s%s", home, base_path, filename);
        printf("Full path: %s\n", full_path); // debug
    } else {
        fprintf(stderr, "Could not determine home directory\n");
    }
}

int load_adjacency_matrix(Road matrix[][MAX_VERTICES], const char *filename, int *num_vertices) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 0;  
    }

    int i = 0, j = 0;
    int n;
    if (fscanf(file, "%d", &n) != 1) {
        perror("Error reading the number of vertices");
        fclose(file);
        return 0;  
    }
    *num_vertices = n;  

    for (int i = 0; i < *num_vertices; i++) {
        for (int j = 0; j < *num_vertices; j++) {
            if (fscanf(file, "%f", &matrix[i][j].weight) != 1) {
                perror("Error reading the matrix values");
                fclose(file);
                return 0;  
            }
            matrix[i][j].state = 0; // Initialization, will be changed after
        }
    }

    printf("Adjacency matrix (%d x %d):\n", *num_vertices, *num_vertices);
    for (i = 0; i < *num_vertices; i++) {
        for (j = 0; j < *num_vertices; j++) {
            printf("%.0f ", matrix[i][j].weight);
        }
        printf("\n");
    }
    printf("\n");

    fclose(file);
    return 1;  
}

void load_graph_from_json(const char *full_path_json, int *num_vertices, Vertex vertices[], Road roads[], int *num_roads) {
    build_path(full_path_json, "Graph_json/", FILENAME_JSON);

    JSON_Value *root_val = json_parse_file(full_path_json);
    JSON_Object *root_obj = json_value_get_object(root_val);

    if (root_val == NULL) {
        fprintf(stderr, "Error parsing JSON file: %s\n", full_path_json);
        return;
    }    

    JSON_Array *ids = json_object_get_array(root_obj, "nodesIds");
    *num_vertices = json_array_get_count(ids);
    *num_roads = 0;

    for (int i = 0; i < *num_vertices; i++) {
        const char *id = json_array_get_string(ids, i);
        JSON_Object *node = json_object_get_object(root_obj, id);

        strcpy(vertices[i].id, id);
        vertices[i].x = (float)json_object_get_number(node, "x");
        vertices[i].y = (float)json_object_get_number(node, "y");

        JSON_Array *adj = json_object_get_array(node, "adjencyList");
        for (int j = 0; j < json_array_get_count(adj); j++) {
            JSON_Array *conn = json_array_get_array(adj, j);
            const char *to = json_array_get_string(conn, 0);
            double weight = json_array_get_number(conn, 1);

            Vertex *start_v = find_vertex_by_id(id, *num_vertices, vertices);
            Vertex *end_v = find_vertex_by_id(to, *num_vertices, vertices);
            if (start_v && end_v) {
                roads[*num_roads].start = start_v - vertices;
                roads[*num_roads].end = end_v - vertices;
                roads[*num_roads].weight = weight;
                roads[*num_roads].road_capacity = (int)(weight / 2);
                (*num_roads)++;
            }
        }
    }

    json_value_free(root_val);
}

#endif