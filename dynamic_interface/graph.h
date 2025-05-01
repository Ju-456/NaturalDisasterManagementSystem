#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100
#define FILENAME_TXT "graphL2.txt"
#define FILENAME_JSON "graphL2.json"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <time.h>

#include <unistd.h>  
#include <limits.h> 
#include <ctype.h>

#include <raylib.h>
#include "parson.h"

typedef struct {
    char id[3];  
    float x;
    float y;              
    int degree;               // (arcs sortants)
    int type;                 // 0: city, 1: hospital, 2: warehouse
    int type_of_storage;      // 0: food, 1: medicine, 2: other resources
    int storage_capacity;     // in kg(s) = degree * 2
} Vertex;

typedef struct {
    int start;         
    int end;           
    int state;         // 1: bad, 2: good, 3: very
    float weight;      // from the Adj matrix file
    int road_capacity; // road_capacity = weight/2 = number of vehiculs which can pass at the same time
} Road;

typedef struct {
    int ambulance;
    int truck;
    int vehicle_capacity;
} Vehicle;

#ifndef PATH_MAX
#define PATH_MAX 4096 
#endif

// General functions
void build_path(char *full_path, const char *base_path, const char *filename);
int load_adjacency_matrix(Road matrix[][MAX_VERTICES], const char *filename, int *num_vertices);
void read_ids_from_json(const char *full_path_json, Vertex vertices[], int *num_vertices);
void load_graph_from_json(const char *full_path_json, int *num_vertices, Vertex vertices[], Road roads[], int *num_roads);

// Earthsquake's part
int earthquake (int num_vertices, Road matrix[][100]);

#endif