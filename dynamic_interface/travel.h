#ifndef TRAVEL_H
#define TRAVEL_H

#include "graph.h"

// Initilization part
void init_city_need(int num_vertices,Road matrix[][MAX_VERTICES],Vertex vertices[MAX_VERTICES]);
void init_type_of_issue(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES]);
void init_travel_time(int num_vertices, Road matrix[][MAX_VERTICES]);
void display_info_travel(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES]);

// Execution of the travel
int research_closest_vertex(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start, int target_type);
void display_research_closest_vertex(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[], int start, int target_type);
int travel_to_city(int num_vertices, Road matrix[][MAX_VERTICES], Vertex vertices[MAX_VERTICES], int start);

#endif