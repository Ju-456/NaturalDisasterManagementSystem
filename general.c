#ifndef GENERAL
#define GENERAL

#include "graph.h"

// finding user path and build to access to graph1.txt
void build_path(char *full_path) {
    char current_directory[PATH_MAX];
    const char *base_folder = "MatAdj_txt";

    if (getcwd(current_directory, PATH_MAX) != NULL) {
        snprintf(full_path, 512, "%s/%s/%s", current_directory, base_folder, FILENAME);
        printf(" Full path to file: %s\n", full_path);  // pour debug
    }
}

// reading
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

    for (i = 0; i < *num_vertices; i++) {
        for (j = 0; j < *num_vertices; j++) {
            matrix[i][j].weight = 0.0f;
            matrix[i][j].state = 0;
        }
    }

    for (i = 0; i < *num_vertices; i++) {
        for (j = 0; j < *num_vertices; j++) {
            if (fscanf(file, "%f", &matrix[i][j].weight) != 1) {
                perror("Error reading the matrix values");
                fclose(file);
                return 0;  
            }
            matrix[i][j].road_capacity = matrix[i][j].weight / 2;
        }
    }

    printf("Adjacency matrix (%d x %d):\n", *num_vertices, *num_vertices);
        for (int i = 0; i < *num_vertices; i++) {
            for (int j = 0; j < *num_vertices; j++) {
                printf("%.0f ", matrix[i][j].weight);
            }
            printf("\n");
    }
    printf("\n");

    fclose(file);
    return 1;  
}

#endif