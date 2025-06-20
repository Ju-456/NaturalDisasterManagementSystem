#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100
#define FILENAME_TXT "graphXL.txt"
#define FILENAME_JSON "graphXL.json"
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
    int predecessor[MAX_VERTICES];
    int nextVertex[MAX_VERTICES];
    int shortestPath[MAX_VERTICES]; // The way TO the vertex it needs (e.g. : city -> hopital)
    int closest;
} Vertex;

typedef struct {
    char id[MAX_VERTICES];
    int cfc_group;
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

typedef struct { float x, y; } Point;
#ifndef PATH_MAX
#define PATH_MAX 4096 
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TOLERANCE 20.0f

typedef enum {
    MODE_GRAPH,
    MODE_VERTEX_DETAILS,
    MODE_ROAD_DETAILS
} AppMode;

// General functions
void build_path(char *full_path, const char *base_path, const char *filename);
int load_adjacency_matrix(Road matrix[][MAX_VERTICES], const char *filename, int *num_vertices);
void read_id_from_json(const char *full_path_json, Vertex vertices[], int *num_vertices);
void load_graph_from_json(const char *full_path_json, int *num_vertices, Vertex vertices[], Road roads[], int *num_roads);

// Earthsquake's part
int earthquake (int num_vertices, Road matrix[][MAX_VERTICES], int num_roads);

// Shortest's path part
int verif(int *isMarked, int num_vertices);
int min(int num_vertices, Road matrix[][MAX_VERTICES], int i);
void dijkstra(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start);
void display_dijkstra(int num_vertices, Vertex vertices[], int start);
int build_path_points(int num_vertices, Vertex vertices[], int start, int target, Point* path_points, int* point_count);

void edmonds(Road matrix[][MAX_VERTICES], Vertex vertices[], int n, int roads[][MAX_VERTICES]); 

#endif