#include "graph.h"
#include "texture.h"
#include "window_draw_part.h"
#include "window_general_part.h"
#include "raymath.h"

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

// Gestion of car's direction
Rectangle get_blue_car_sprite(Direction dir) {
    switch (dir) {        
        case LEFT:  return (Rectangle){1, 103, 49, 18};   // ←
        case RIGHT: return (Rectangle){1, 86, 49, 15};    // →

        case DOWN:  return (Rectangle){53, 81, 14, 38};   // ↓
        case UP:    return (Rectangle){70, 81, 15, 37};   // ↑
        
        default:    return (Rectangle){1, 86, 49, 15};    // fallback →
    }
}

Rectangle get_green_car_sprite(Direction dir) {
    switch (dir) {
        case LEFT:  return (Rectangle){6, 123, 46, 17};   // ←
        case RIGHT: return (Rectangle){6, 143, 46, 16};   // →

        case DOWN:  return (Rectangle){54, 121, 15, 32};  // ↓
        case UP:    return (Rectangle){71, 121, 15, 38};  // ↑

        default:    return (Rectangle){6, 143, 46, 16};   // fallback →
    }
}

Direction get_direction(float dx, float dy) {
    float abs_dx = fabsf(dx);
    float abs_dy = fabsf(dy);
    float tolerance = 0.5f;

    Direction dir;

    if (abs_dx > abs_dy + tolerance) {
        dir = (dx > 0) ? RIGHT : LEFT;
    } else if (abs_dy > abs_dx + tolerance) {
        dir = (dy > 0) ? DOWN : UP;
    } else {
        dir = (dy > 0) ? DOWN : UP;
    }

    printf("dx=%.2f dy=%.2f => direction=%s\n", dx, dy,
           dir == UP ? "UP" : dir == DOWN ? "DOWN" : dir == LEFT ? "LEFT" : "RIGHT");

    return dir;
}

// Choice of car's color
Rectangle get_sprite_for_direction(Direction dir, int index, Vertex vertices[]) {
    if (vertices[index].issue == 1) return get_blue_car_sprite(dir);
    if (vertices[index].issue == 2) return get_green_car_sprite(dir);
    return get_blue_car_sprite(dir); // fallback
}

void draw_travel_effects(int num_vertices, Vertex vertices[], int index, Texture2D voitures) {
    int closest = vertices[index].closest;
    if (closest == -1 || vertices[index].need != 1) return;

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    float scale = fminf((float)screen_width / SCREEN_WIDTH, (float)screen_height / SCREEN_HEIGHT);
    float offsetX = (screen_width - SCREEN_WIDTH * scale) / 2.0f;
    float offsetY = (screen_height - SCREEN_HEIGHT * scale) / 2.0f;

    static double start_time[MAX_VERTICES] = {0};
    static bool animation_started[MAX_VERTICES] = {false};

    if (!animation_started[index]) {
        start_time[index] = GetTime();
        animation_started[index] = true;
    }

    double elapsed = GetTime() - start_time[index];

    int text_w = MeasureText(TextFormat("Intervention: %s -> %s", vertices[index].id, vertices[closest].id), 18);
    DrawRectangle(15, 15, text_w + 10, 25, LIGHTGRAY);
    DrawRectangleLines(15, 15, text_w + 10, 25, DARKGRAY);
    DrawText(TextFormat("Intervention: %s -> %s", vertices[index].id, vertices[closest].id), 20, 20, 18, BLACK);

    Point route[MAX_VERTICES];
    int point_count = build_path_points(num_vertices, vertices, index, closest, route, &point_count);
    if (point_count == 0) {
        DrawText("Path doesn't exist", 20, 50, 18, ORANGE);
        vertices[index].issue = vertices[index].need = 0;
        animation_started[index] = false;
        return;
    }

    bool reverse = (vertices[index].type == 1 || vertices[index].type == 2) && vertices[closest].type == 0;

    if (vertices[index].type == 0 && vertices[closest].type == 0) {
        vertices[index].issue = vertices[index].need = 0;
        animation_started[index] = false;
        return;
    }

    draw_car_animation(route, point_count, reverse, (Rectangle){0, 0, 0, 0},
                   voitures, true, elapsed, scale, offsetX, offsetY, index, vertices);

    if (elapsed >= 6.0f) {
        vertices[index].issue = vertices[index].need = 0;
        animation_started[index] = false;
    }
}

void draw_car_animation(Point route[], int point_count, bool reverse, Rectangle src_unused, Texture2D voitures,
                        bool rotate, double elapsed, float scale, float offsetX, float offsetY,
                        int index, Vertex vertices[]) {

    float total_duration = 3.0f;
    float vehicle_scale = 1.0f;

    // Scale and offset each point on the path
    for (int i = 0; i < point_count; i++) {
        route[i].x = route[i].x * scale + offsetX;
        route[i].y = route[i].y * scale + offsetY;
    }

    if (elapsed < total_duration) {
        float t = elapsed / total_duration;
        if (t > 1.0f) t = 1.0f;

        float p = t * (point_count - 1);
        int k = (int)p;
        float f = p - k;
        if (k >= point_count - 1) k = point_count - 2;

        Point a = reverse ? route[point_count - 1 - k] : route[k];
        Point b = reverse ? route[point_count - 2 - k] : route[k + 1];

        float dx = b.x - a.x;
        float dy = b.y - a.y;

        Rectangle src;
        float angle = 0.0f;

        if (dx != 0 && dy != 0) {
            Direction base_dir = (dx > 0) ? RIGHT : LEFT;
            src = get_sprite_for_direction(base_dir, index, vertices);
            angle = rotate ? ((dy < 0) ? 45.0f : -45.0f) : 0.0f;
        } else {
            Direction dir = get_direction(dx, dy);
            src = get_sprite_for_direction(dir, index, vertices);

            if (rotate) {
                switch (dir) {
                    case UP:    angle = -180.0f; break;
                    case DOWN:  angle = 180.0f; break;
                    default:    angle = atan2f(dy, dx) * RAD2DEG; break;
                }
            }
        }

        Rectangle dest = {
            a.x + f * dx - src.width * vehicle_scale / 2,
            a.y + f * dy - src.height * vehicle_scale / 2,
            src.width * vehicle_scale,
            src.height * vehicle_scale
        };

        DrawTexturePro(voitures, src, dest,
            (rotate ? (Vector2){src.width * vehicle_scale / 2, src.height * vehicle_scale / 2} : (Vector2){0, 0}),
            angle, WHITE);
    }

    else if (elapsed < 6.0f) {
        Point last = reverse ? route[0] : route[point_count - 1];
        Point before_last = reverse ? route[1] : route[point_count - 2];

        float dx = last.x - before_last.x;
        float dy = last.y - before_last.y;

        Rectangle src;
        float angle = 0.0f;

        if (dx != 0 && dy != 0) {
            Direction base_dir = (dx > 0) ? RIGHT : LEFT;
            src = get_sprite_for_direction(base_dir, index, vertices);
            angle = rotate ? ((dy < 0) ? 45.0f : -45.0f) : 0.0f;
        } else {
            Direction dir = get_direction(dx, dy);
            src = get_sprite_for_direction(dir, index, vertices);

            if (rotate) {
                switch (dir) {
                    case UP:    angle = -180.0f; break;
                    case DOWN:  angle = 180.0f; break;
                    default:    angle = atan2f(dy, dx) * RAD2DEG; break;
                }
            }
        }

        Rectangle dest = {
            last.x - src.width * vehicle_scale / 2,
            last.y - src.height * vehicle_scale / 2,
            src.width * vehicle_scale,
            src.height * vehicle_scale
        };

        DrawTexturePro(voitures, src, dest,
            (rotate ? (Vector2){src.width * vehicle_scale / 2, src.height * vehicle_scale / 2} : (Vector2){0, 0}),
            angle, WHITE);
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

void draw_dashed_line_ex(Vector2 start, Vector2 end, float thickness, float dashLength, Color color) {
    Vector2 direction = Vector2Subtract(end, start);
    float totalLength = Vector2Length(direction);
    direction = Vector2Normalize(direction);

    float drawn = 0.0f;
    bool draw = true;

    while (drawn < totalLength) {
        float segmentLength = fminf(dashLength, totalLength - drawn);
        Vector2 segStart = Vector2Add(start, Vector2Scale(direction, drawn));
        Vector2 segEnd = Vector2Add(segStart, Vector2Scale(direction, segmentLength));

        if (draw) {
            DrawLineEx(segStart, segEnd, thickness, color);
        }

        drawn += segmentLength;
        draw = !draw;
    }
}

void draw_min_spanning_tree(int num_vertices, Road matrix[][MAX_VERTICES], Vertex *vertices, int roads[][MAX_VERTICES]) {
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

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (roads[j][i] == 1) {
                Vector2 start = { scaled_vertices[i].x, scaled_vertices[i].y };
                Vector2 end = { scaled_vertices[j].x, scaled_vertices[j].y };

                draw_dashed_line_ex(start, end, 3.0f, 10.0f, (Color){100, 230, 100, 255});
                // draw_dashed_line_ex(start, end, 3.0f, 10.0f, BLUE);
            }
        }
    }
}