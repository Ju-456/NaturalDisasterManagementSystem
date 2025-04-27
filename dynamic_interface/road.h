#ifndef ROAD_H
#define ROAD_H

#include "graph.h"

// Road's part
int count_roads(Road roads[], Road matrix[][100], int num_vertices);
void init_roads_characteristics(Road matrix[][100], int num_vertices);
int display_roads_state_matrix(Road matrix[][100], int num_vertices);
char *get_type_name(int type);
void display_roads_characteristics(Vertex vertices[], Road roads[], int num_roads);

#endif