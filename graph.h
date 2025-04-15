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
    int start;         // start
    int end;           // end
    int state;         // 1: bad, 2: good, 3: very
    float weight;      // from the Adj matrix file
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

// Vertex's part
void count_vertex_degree(Vertex vertices[], Road matrix[][100], int num_vertices);
void sort_vertices(Vertex vertices[], int num_vertices);
void init_vertex_characteristics(Vertex vertices[], Road matrix[][100], int num_vertices);
void display_vertex_characteristics(Vertex vertices[], int num_vertices);

// Goad's part
int count_roads(Road roads[], Road matrix[][100], int num_vertices);
void init_roads_characteristics(Road matrix[][100], int num_vertices);
int display_roads_state_matrix(Road matrix[][100], int num_vertices);
char *get_type_name(int type);
void display_roads_characteristics(Vertex vertices[], Road roads[], int num_roads);

// Earthsquake's part
int earthquake (int num_vertices, Road matrix[][100]);

#endif