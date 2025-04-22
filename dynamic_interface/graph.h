#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100
#define FILENAME_TXT "graph1.txt"
#define FILENAME_JSON "graph1.json"

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


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TOLERANCE 10.0f

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

typedef enum {
    MODE_GRAPH,
    MODE_VERTEX_DETAILS
} AppMode;

#ifndef PATH_MAX
#define PATH_MAX 4096 
#endif

// General functions
void build_path(char *full_path, const char *base_path, const char *filename);
int load_adjacency_matrix(Road matrix[][MAX_VERTICES], const char *filename, int *num_vertices);
void load_graph_from_json(const char *full_path_json, int *num_vertices, Vertex vertices[], Road roads[], int *num_roads);

// Vertex's part
void generate_vertex_id(int index, char* id_out);
void count_vertex_degree(Vertex vertices[], Road matrix[][100], int num_vertices);
void sort_vertices(Vertex vertices[], int num_vertices);
void init_vertex_characteristics(Vertex vertices[], Road matrix[][100], int num_vertices);
void display_vertex_characteristics(Vertex vertices[], int num_vertices);

// Road's part
int count_roads(Road roads[], Road matrix[][100], int num_vertices);
void init_roads_characteristics(Road matrix[][100], int num_vertices);
int display_roads_state_matrix(Road matrix[][100], int num_vertices);
char *get_type_name(int type);
void display_roads_characteristics(Vertex vertices[], Road roads[], int num_roads);

// Earthsquake's part
int earthquake (int num_vertices, Road matrix[][100]);

// Window's part
Vertex *find_vertex_by_id(const char *id, int num_vertices, Vertex vertices[]);
bool are_connected(const char *id1, const char *id2, int num_roads, Road roads[], Vertex vertices[]);

void draw_roads_with_orientation(int num_vertices, Vertex vertices[], Road roads[], int num_roads);
void draw_vertices_with_type(int num_vertices, Vertex *vertices);

void init_window_vertex(Vertex *vertices, Vertex *scaled_vertices, int num_vertices, AppMode *mode, int *selected_index);
//void ini_window_road
void init_window_custom(const char *filename, int num_vertices, Vertex *vertices, Road *roads, int num_roads);

#endif