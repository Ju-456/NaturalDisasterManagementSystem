#include "graph.h"

// to determin the vertex's type
void count_vertex_degree(Vertex vertices[], Road matrix[][MAX_VERTICES], int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        // generate_vertex_id(i, vertices[i].id); // use directly id from json bc it'more reliable
        vertices[i].degree = 0;
        for (int j = 0; j < num_vertices; j++) {
            if (matrix[i][j].weight > 0) {
                vertices[i].degree++;
            }
        }
    }
}

void init_vertex_characteristics(Vertex vertices[], Road matrix[][MAX_VERTICES], int num_vertices) {
    count_vertex_degree(vertices, matrix, num_vertices);
    
    int indices[MAX_VERTICES];
    for (int i = 0; i < num_vertices; i++) {
        indices[i] = i;
    }

    srand(time(NULL));
    for (int i = num_vertices - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    int hospitals_count = num_vertices * 0.2;
    int warehouses_count = num_vertices * 0.2;

    for (int i = 0; i < num_vertices; i++) {
        vertices[i].type = 0; // default: city
    }

    for (int i = 0; i < hospitals_count; i++) {
        vertices[indices[i]].type = 1; // hospital
    }

    for (int i = hospitals_count; i < hospitals_count + warehouses_count; i++) {
        vertices[indices[i]].type = 2; // warehouse
    }

    for (int i = 0; i < num_vertices; i++) {
        vertices[i].storage_capacity = vertices[i].degree * 2;
    }
}

void attribute_vertex_characteristics(Vertex vertices[], int num_vertices, int *hospitals, int *cities, int *warehouses) {
    *hospitals = 0;
    *cities = 0;
    *warehouses = 0;

    for (int i = 0; i < num_vertices; i++) {
        if (vertices[i].type == 1) (*hospitals)++;
        else if (vertices[i].type == 0) (*cities)++;
        else if (vertices[i].type == 2) (*warehouses)++;
    }

    // Sort vertices alphabetically by id
    for (int i = 0; i < num_vertices - 1; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            if (strcmp(vertices[i].id, vertices[j].id) > 0) {
                Vertex temp = vertices[i];
                vertices[i] = vertices[j];
                vertices[j] = temp;
            }
        }
    }
}

void display_vertex_characteristics(Vertex vertices[], int num_vertices) {
    int hospitals, cities, warehouses;

    attribute_vertex_characteristics(vertices, num_vertices, &hospitals, &cities, &warehouses);

    printf("Information about Vertex's Map:\n\n");
    printf("There is %d hospitals, %d cities and %d warehouses.\n\n", hospitals, cities, warehouses);

    for (int i = 0; i < num_vertices; i++) {
        printf("%s:\n", vertices[i].id);

        switch (vertices[i].type) {
            case 0: printf("type: city\n"); break;
            case 1: printf("type: hospital\n"); break;
            case 2: printf("type: warehouse\n"); break;
        }

        printf("nb of roads: %d\n", vertices[i].degree);

        printf("storage capacity: %d kgs\n", vertices[i].storage_capacity);
        printf("----------------------------------\n");
    }
}
