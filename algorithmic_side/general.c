#ifndef GENERAL
#define GENERAL

#include "graph.h"

void build_path(char *full_path) {
    const char *home = getenv("HOME");  // recuperation of user name
    const char *base_path = "/NaturalDisasterManagementSystem/MatAdj_txt/";

    if (home != NULL) {
        snprintf(full_path, PATH_MAX, "%s%s%s", home, base_path, FILENAME);
        printf(" Full path to file: %s\n", full_path);  // pour debug
    } else {
        fprintf(stderr, "Could not determine home directory\n");
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

#endif