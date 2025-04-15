#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100
#define FILENAME "graph1.txt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  
#include <limits.h> 

#ifndef PATH_MAX
#define PATH_MAX 4096 
#endif

typedef struct {
    char id[2];                
    int degree;               // (arcs sortants)
    int type;                 // 0: city, 1: hospital, 2: warehouse
    int type_of_storage;      // 0: food, 1: medicine, 2: other resources
    int storage_capacity;     // in kg(s) = degree * 2
} Vertex;

typedef struct {
    int state;        // 0, 1, 2
    float weight;     // from the Adj matrix file
    int road_capacity; // road_capacity = weight/2
} Road;

typedef struct {
    int ambulance;
    int truck;
    int vehicle_capacity;
} Vehicle;

// general functions
void build_path(char *full_path);  
int load_adjacency_matrix(Road matrix[][MAX_VERTICES], const char *filename, int *num_vertices);

// vertex's part
void count_vertex_degree(Vertex vertices[], Road matrix[][100], int num_vertices);
void sort_vertices(Vertex vertices[], int num_vertices);
void init_vertex_characteristics(Vertex vertices[], Road matrix[][100], int num_vertices);
void display_vertex_characteristics(Vertex vertices[], int num_vertices);

#endif