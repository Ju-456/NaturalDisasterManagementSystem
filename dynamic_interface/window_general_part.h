#ifndef WINDOW_H
#define WINDOW_H

#include "graph.h"

#include "texture.h"
#include "window_general_part.h"

// Window's part
Vertex *find_vertex_by_id(const char *id, int num_vertices, Vertex vertices[]);
bool are_connected(const char *id1, const char *id2, int num_roads, Road roads[], Vertex vertices[]);

void button_click(bool *menu_open, bool *show_states, int num_vertices, Vertex *vertices, Road roads[], int num_roads,
    Texture2D transition_texture, Texture2D grass_texture, const char *message, Road matrix[][MAX_VERTICES], int order_for_intervention);

void init_window_vertex(Vertex *vertices, Vertex *scaled_vertices, int num_vertices, AppMode *mode, int *selected_index);
void init_window_road(Vertex *original_vertices, Vertex *scaled_vertices, Road *roads, int num_roads, AppMode *mode, int *selected_index, Road matrix[][MAX_VERTICES]);
void init_window_custom(const char *filename, int num_vertices, Vertex *vertices, Road *roads, int num_roads, Road matrix[][MAX_VERTICES], int order_for_intervention);

#endif
