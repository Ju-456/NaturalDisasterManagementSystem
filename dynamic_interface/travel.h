#ifndef TRAVEL_H
#define TRAVEL_H

#include "graph.h"

void init_city_need(int num_vertices,Road matrix[][MAX_VERTICES],Vertex vertices[MAX_VERTICES]);
void init_type_of_issue(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES]);
void init_travel_time(int num_vertices, Road matrix[][MAX_VERTICES]);
void display_info_travel(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES]);

#endif