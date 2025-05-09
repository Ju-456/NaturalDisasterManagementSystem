#ifndef ROAD_H
#define ROAD_H

#include "graph.h"

// Road's part
int count_roads(Road roads[], Road matrix[][MAX_VERTICES], int num_vertices);
void init_roads_characteristics(Road matrix[][MAX_VERTICES], int num_vertices);
char *get_type_name(int type);
void init_roads_id(Vertex vertices[], Road roads[], int num_roads);

int display_roads_state_matrix(Road matrix[][MAX_VERTICES], int num_vertices);
void display_roads_characteristics(Vertex vertices[], Road roads[], int num_roads);

#endif