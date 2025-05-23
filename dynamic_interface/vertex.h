#ifndef VERTEX_H
#define VERTEX_H

#include "graph.h"

// Vertex's part
void count_vertex_degree(Vertex vertices[], Road matrix[][MAX_VERTICES], int num_vertices);
void sort_vertices(Vertex vertices[], int num_vertices);
void init_vertex_characteristics(Vertex vertices[], Road matrix[][MAX_VERTICES], int num_vertices);
void attribute_vertex_characteristics(Vertex vertices[], int num_vertices, int *hospitals, int *cities, int *warehouses);
void display_vertex_characteristics(Vertex vertices[], int num_vertices);

#endif