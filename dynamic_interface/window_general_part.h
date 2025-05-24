#ifndef WINDOW_GENERAL_PART_H
#define WINDOW_GENERAL_PART_H

#include "graph.h"
#include "road.h"

#include "travel.h"
#include "window_draw_part.h"
#include "vertexGroup.h"

Vertex *find_vertex_by_id(const char *id, int num_vertices, Vertex vertices[]);
bool are_connected(const char *id1, const char *id2, int num_roads, Road roads[], Vertex vertices[]);

// Buttons's part
void buttons_click_logic(bool *menu_open, bool *show_states, bool *show_group_vertices, int num_vertices, Vertex *vertices, Road roads[], int num_roads, 
                         Texture2D transition_texture, Texture2D grass_texture, Road matrix[][MAX_VERTICES], int *order_for_intervention, bool *showMinST);
void buttons_click_draw(bool menu_open, bool show_states, bool show_group_vertices, int num_vertices, Vertex *vertices, Road matrix[][MAX_VERTICES]);

void init_window_vertex(Vertex *vertices, Vertex *scaled_vertices, int num_vertices, AppMode *mode, int *selected_index);
void init_window_road(Vertex *original_vertices, Vertex *scaled_vertices, Road *roads, int num_roads, AppMode *mode, int *selected_index, Road matrix[][MAX_VERTICES]);
void init_window_custom(const char *filename, int num_vertices, Vertex *vertices, Road *roads, int num_roads, 
    Road matrix[][MAX_VERTICES], int order_for_intervention);

#endif
