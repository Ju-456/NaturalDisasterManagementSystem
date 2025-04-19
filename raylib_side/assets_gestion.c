#include "animation.h"

void build_path_asset(char *full_path, const char *filename) {
    const char *base_path = "City_Tilemap/";

    snprintf(full_path, PATH_MAX, "%s%s", base_path, filename);
    printf("Full path to file: %s\n", full_path);  // pour debug
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

/*void build_road(){

}

void vertex_assignment(){

    Rectangle hospital = { 23, 64, 23, 64 };
    Rectangle warehouse = { 64, 56, 64, 56};
    Rectangle city = { 64, 64, 64, 64};
    Rectangle road = { 30, 117, 30, 117};
}*/