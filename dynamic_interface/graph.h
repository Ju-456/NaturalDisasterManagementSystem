#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100
#define FILENAME_TXT "graphL1.txt"
#define FILENAME_JSON "graphL1.json"
#define INF INT_MAX

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
    int storage_capacity;     // in kg(s) = degree * 2
    int need;                 // 0: all fine 1: issue to fix !
    int issue;               // 1: need hospital, 2: need warehouse (after earthquake, vertex need resources)
    int nextVertex[MAX_VERTICES];
    int shortestPath[MAX_VERTICES]; // The way TO the vertex it needs (e.g. : city -> hopital)
} Vertex;

typedef struct {
    int start;         
    int end;           
    int state;         // 1: bad, 2: good, 3: very
    float weight;      // from the Adj matrix file
    int road_capacity; // road_capacity = weight/2 = number of vehiculs which can pass at the same time
    int travel_time;   // depends of state and weight
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
void read_id_from_json(const char *full_path_json, Vertex vertices[], int *num_vertices);
void load_graph_from_json(const char *full_path_json, int *num_vertices, Vertex vertices[], Road roads[], int *num_roads);

// Earthsquake's part
int earthquake (int num_vertices, Road matrix[][MAX_VERTICES]);

//shortest path part
void dijkstra(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start);

#endif