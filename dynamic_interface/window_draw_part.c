#include "graph.h"
#include "texture.h"
#include "window_draw_part.h"
#include "window_general_part.h"

void draw_roads_with_orientation(int num_vertices, Vertex vertices[], Road roads[], int num_roads) {
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            Vertex *a = &vertices[i];
            Vertex *b = &vertices[j];

            if (!are_connected(a->id, b->id, num_roads, roads, vertices)) continue;

            // HORIZONTAL
            if (fabsf(a->y - b->y) < TOLERANCE) {
                float minX = fminf(a->x, b->x);
                float maxX = fmaxf(a->x, b->x);
                float y = (a->y + b->y) / 2.0f;

                Rectangle src = {80, 129, 130, 30}; // Horizontal road tile
                Rectangle dest = {
                    minX, y - 16,
                    maxX - minX, 32
                };

                DrawTexturePro(road_texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
            }

            // VERTICAL
            else if (fabsf(a->x - b->x) < TOLERANCE) {
                float minY = fminf(a->y, b->y);
                float maxY = fmaxf(a->y, b->y);
                float x = (a->x + b->x) / 2.0f;

                Rectangle src = {30, 0, 30, 128}; // Vertical road tile
                Rectangle dest = {
                    x - 16, minY,
                    32, maxY - minY
                };

                DrawTexturePro(road_texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
            }
            // DIAGONAL
            else {
                float dx = b->x - a->x;
                float dy = b->y - a->y;
                float angle = -atan2f(dy, dx) * RAD2DEG;
                float distance = sqrtf(dx * dx + dy * dy);
            
                Vector2 center = {
                    (a->x + b->x) / 2.0f,
                    (a->y + b->y) / 2.0f
                };
            
                // Taille visuelle
                float visualWidth = 32.0f;  
                float visualLength = distance;
            
                Rectangle src = {30, 0, 30, 128};
            
                Rectangle dest = {
                    center.x, center.y,
                    visualWidth, visualLength
                };
            
                Vector2 origin = {visualWidth / 2, visualLength / 2};
            
                DrawTexturePro(road_texture, src, dest, origin, angle, WHITE);
            }            
        }
    }
}

void draw_state_for_existing_roads(int num_vertices, Vertex vertices[], Road matrix[][MAX_VERTICES], Road roads[], int num_roads) {
    static bool first_call = true; // to prevent having orange without changing states
    int k = 0;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            Vertex *a = &vertices[i];
            Vertex *b = &vertices[j];

            if (!are_connected(a->id, b->id, num_roads, roads, vertices)) continue;

            // display only existing roads
            if (matrix[i][j].weight <= 0) continue;

            Road road_before = roads[k];
            int state_before = road_before.state;
            int state_after = matrix[i][j].state;
            // printf("%d ", matrix[i][j].state);
            k++;
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%d", matrix[i][j].state);  

            Vector2 center = {
                (a->x + b->x) / 2.0f,
                (a->y + b->y) / 2.0f
            };

            Color color;
            if (first_call) {
                color = WHITE; 
                first_call = false; 
            }
            else {
                if ((state_before == state_after)) {
                    color = WHITE;
                } else if ((state_before != state_after)){
                    color = ORANGE;
                }
            }
            // printf("Checking road between %d and %d\n", a->id, b->id);
            // printf("state_before = %d, state_after = %d\n", state_before, state_after);

            DrawText(buffer, (int)(center.x - MeasureText(buffer, 14) / 2), (int)(center.y - 7), 14, color);
        }
    }
}

void draw_vertices_with_type(int num_vertices, Vertex *vertices) {
    for (int i = 0; i < num_vertices; i++) {
        Vertex *vertex = &vertices[i];
        Vector2 pos = { vertex->x - 15, vertex->y - 35 }; // positioning + top and + left, esthetism
        
        Rectangle src;

        switch (vertex->type) {
            case 0: // City
                // src = (Rectangle){64, 64, 64, 63}; 
                src = (Rectangle){352, 63, 64, 63}; // To add diverty of colors in the map
                break;
            case 1: // Hospital
                src = (Rectangle){287, 64, 64, 63}; 
                break;
            case 2: // Warehouse
                src = (Rectangle){416, 64, 64, 63}; 
                break;
            default:
                src = (Rectangle){0, 0, 32, 32}; // Texture fallback par défaut
                break;
        }

        // scale factor for the building's size
        float scale_factor = 0.8f;

        Rectangle dest = {
            pos.x,
            pos.y,
            src.width * scale_factor,  
            src.height * scale_factor  
        };

        DrawTexturePro(road_texture, src, dest, (Vector2){0, 0}, 0.0f, WHITE);

        //DrawText(vertex->id, pos.x + 5, pos.y - 10, 10, BLACK); // to see the other vertices who still don't have building
    }
}

void draw_travel_effects(int num_vertices, Vertex vertices[], int index) {
    int closest_vertex = vertices[index].closest;
    if (closest_vertex == -1 || vertices[index].need != 1) return;

    static double start_time[MAX_VERTICES] = {0};
    static bool animation_started[MAX_VERTICES] = {false};

    if (!animation_started[index]) {
        start_time[index] = GetTime();
        animation_started[index] = true;
    }

    double elapsed = GetTime() - start_time[index];

    int text_width = MeasureText(TextFormat("Intervention: %s -> %s", vertices[index].id, vertices[closest_vertex].id), 18);
    DrawRectangle(15, 15, text_width + 10, 25, LIGHTGRAY);
    DrawRectangleLines(15, 15, text_width + 10, 25, DARKGRAY);
    DrawText(TextFormat("Intervention: %s -> %s", vertices[index].id, vertices[closest_vertex].id), 20, 20, 18, BLACK);

    Point route_points[MAX_VERTICES];
    int point_count = build_path_points(num_vertices, vertices, index, closest_vertex, route_points, &point_count);

    if (point_count == 0) {
        DrawRectangle(15, 15, text_width + 10, 25, LIGHTGRAY);
        DrawRectangleLines(15, 15, text_width + 10, 25, DARKGRAY);
        DrawText("Path doesn't exist", 20, 50, 18, ORANGE);
        vertices[index].issue = 0;
        vertices[index].need = 0;
        animation_started[index] = false;
        return;
    }

    float total_duration = 3.0f;
    float step_spacing = 0.2f;

    bool reverse_animation = false;

    if ((vertices[index].type == 1 || vertices[index].type == 2) && vertices[closest_vertex].type == 0) { //if h/w -> city
        reverse_animation = true;
    } else if (vertices[index].type == 0 && vertices[closest_vertex].type == 0) { // no display, this case shouldn't exist normally
        vertices[index].issue = 0;
        vertices[index].need = 0;
        animation_started[index] = false;
        return;
    }

    // === Animation ===
    if (elapsed < total_duration) {
        int steps = (int)(elapsed / step_spacing);
        for (int s = 0; s <= steps; s++) {
            float t = (s * step_spacing) / total_duration;
            if (t > 1.0f) t = 1.0f;

            float p = t * (point_count - 1);
            int k = (int)p;
            float f = p - k;
            if (k >= point_count - 1) k = point_count - 2;

            float x, y;

            if (reverse_animation) {
                x = route_points[point_count - 1 - k].x + f * (route_points[point_count - 2 - k].x - route_points[point_count - 1 - k].x);
                y = route_points[point_count - 1 - k].y + f * (route_points[point_count - 2 - k].y - route_points[point_count - 1 - k].y);
            } else {
                x = route_points[k].x + f * (route_points[k + 1].x - route_points[k].x);
                y = route_points[k].y + f * (route_points[k + 1].y - route_points[k].y);
            }

            DrawCircle(x, y, 4, ORANGE);
        }
    } else if (elapsed < 6.0f) {
        Point last = reverse_animation ? route_points[0] : route_points[point_count - 1];
        DrawCircle(last.x, last.y, 8, GREEN);
    } else {
        vertices[index].issue = 0;
        vertices[index].need = 0;
        animation_started[index] = false;
    }
}

void draw_group_of_vertices(int num_vertices, Road matrix[][MAX_VERTICES], Vertex *vertices) {
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    float scale = fminf((float)screen_width / SCREEN_WIDTH, (float)screen_height / SCREEN_HEIGHT);
    float offsetX = (screen_width - SCREEN_WIDTH * scale) / 2.0f;
    float offsetY = (screen_height - SCREEN_HEIGHT * scale) / 2.0f;

    Vertex scaled_vertices[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
        scaled_vertices[i] = vertices[i];
        scaled_vertices[i].x = vertices[i].x * scale + offsetX;
        scaled_vertices[i].y = vertices[i].y * scale + offsetY;
    }

    Color cfc_colors[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        cfc_colors[i] = BLANK;
    }

    int num_assigned = 0;

    for (int i = 0; i < num_vertices; i++) {
        int group_id = matrix[i][i].cfc_group;

        if (cfc_colors[group_id].r == BLANK.r && cfc_colors[group_id].g == BLANK.g && cfc_colors[group_id].b == BLANK.b) {
            cfc_colors[group_id] = group_colors[num_assigned % (sizeof(group_colors) / sizeof(Color))];
            num_assigned++;
        }

        Color group_color = cfc_colors[group_id];

        DrawCircle(scaled_vertices[i].x, scaled_vertices[i].y, 5, group_color);
        DrawText(TextFormat("%d", i), scaled_vertices[i].x + 10, scaled_vertices[i].y - 10, 12, BLACK);
    }

    for (int i = 0; i < num_vertices; i++) {
        int group_i = matrix[i][i].cfc_group;

        for (int j = 0; j < num_vertices; j++) {
            int group_j = matrix[j][j].cfc_group;

            if (group_i == group_j && matrix[i][j].weight > 0) {
                Color group_color = cfc_colors[group_i];

                Vector2 start = { scaled_vertices[i].x, scaled_vertices[i].y };
                Vector2 end = { scaled_vertices[j].x, scaled_vertices[j].y };
                DrawLineEx(start, end, 2.0f, group_color); 
            }
        }
    }
}