#ifndef WINDOW_DRAW_PART_H
#define WINDOW_DRAW_PART_H

#include "graph.h"

#include "texture.h"
#include "color.h"
#include "window_draw_part.h"

void draw_roads_with_orientation(int num_vertices, Vertex vertices[], Road roads[], int num_roads);
void draw_state_for_existing_roads(int num_vertices, Vertex vertices[], Road matrix[][MAX_VERTICES], Road roads[], int num_roads);
void draw_vertices_with_type(int num_vertices, Vertex *vertices);
void draw_travel_effects(int num_vertices, Vertex vertices[], int index);
void draw_group_of_vertices(int num_vertices, Road matrix[][MAX_VERTICES], Vertex *vertices);
// void draw_car_travel()

#endif