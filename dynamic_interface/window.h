#ifndef WINDOW_H
#define WINDOW_H

#include "graph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TOLERANCE 20.0f

typedef enum {
    MODE_GRAPH,
    MODE_VERTEX_DETAILS,
    MODE_ROAD_DETAILS
} AppMode;

// Window's part
Vertex *find_vertex_by_id(const char *id, int num_vertices, Vertex vertices[]);
bool are_connected(const char *id1, const char *id2, int num_roads, Road roads[], Vertex vertices[]);

void draw_roads_with_orientation(int num_vertices, Vertex vertices[], Road roads[], int num_roads);
void draw_state_for_existing_roads(int num_vertices, Vertex vertices[], Road matrix[][MAX_VERTICES], Road roads[], int num_roads);
void draw_vertices_with_type(int num_vertices, Vertex *vertices);
void draw_travel_effects(int num_vertices, Vertex vertices[], int index);

void button_click(bool *menu_open, bool *show_states, int num_vertices, Vertex *vertices, Road roads[], int num_roads,Texture2D transition_texture, Texture2D grass_texture, const char *message, Road matrix[][MAX_VERTICES], int order_for_intervention);

void init_window_vertex(Vertex *vertices, Vertex *scaled_vertices, int num_vertices, AppMode *mode, int *selected_index);
void init_window_road(Vertex *original_vertices, Vertex *scaled_vertices, Road *roads, int num_roads, AppMode *mode, int *selected_index, Road matrix[][MAX_VERTICES]);
void init_window_custom(const char *filename, int num_vertices, Vertex *vertices, Road *roads, int num_roads, Road matrix[][MAX_VERTICES], int order_for_intervention);

#endif