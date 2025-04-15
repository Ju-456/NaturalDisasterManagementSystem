#include "graph.h"

int main() {
    Road matrix[100][100];
    Vertex vertices[100];
    int num_vertices;
    char full_path[PATH_MAX];

    build_path(full_path);

    if (load_adjacency_matrix(matrix, full_path, &num_vertices)) {
        init_vertex_characteristics(vertices, matrix, num_vertices);
        display_vertex_characteristics(vertices, num_vertices);
    } else {
        printf("Error ! Failed to load the adjacency matrix from the file.\n");
        return 1;
    }

    return 0;
}
