#ifndef WINDOW_DRAW_PART_H
#define WINDOW_DRAW_PART_H

#include "graph.h"

#include "texture.h"
#include "color.h"
#include "window_draw_part.h"

typedef enum {
    RIGHT,
    LEFT,
    DOWN,
    UP,
    DOWN_RIGHT,
    DOWN_LEFT,
    UP_RIGHT,
    UP_LEFT
} Direction;

void draw_roads_with_orientation(int num_vertices, Vertex vertices[], Road roads[], int num_roads);

// State's part
void draw_state_for_existing_roads(int num_vertices, Vertex vertices[], Road matrix[][MAX_VERTICES], Road roads[], int num_roads);

// Travel's part
void draw_vertices_with_type(int num_vertices, Vertex *vertices);
Direction get_direction(float dx, float dy);
Rectangle get_blue_car_sprite(Direction dir);
Rectangle get_green_car_sprite(Direction dir);
Rectangle get_sprite_for_direction(Direction dir, int index, Vertex vertices[]);
void draw_travel_effects(int num_vertices, Vertex vertices[], int index, Texture2D voitures);
void draw_car_animation(Point route[], int point_count, bool reverse, Rectangle src_unused, Texture2D voitures,
                        bool rotate, double elapsed, float scale, float offsetX, float offsetY,int index, Vertex vertices[]);

// Group Acessible Areas's part
void draw_group_of_vertices(int num_vertices, Road matrix[][MAX_VERTICES], Vertex *vertices);

// Minimal Spanning Tree's part
void draw_dashed_line_ex(Vector2 start, Vector2 end, float thickness, float dashLength, Color color);
void draw_min_spanning_tree(int num_vertices, Road matrix[][MAX_VERTICES], Vertex *vertices, int roads[][MAX_VERTICES]) ;

#endif