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

    Rectangle src;
    bool rotate = false;

    if (vertices[index].type == 0 && vertices[closest].type == 1)
    {
        if (vertices[index].x == vertices[closest].x)
        {
            if (vertices[index].y > vertices[closest].y)
            {
                src = (Rectangle){52, 81, 16, 38};
            }
            else if (vertices[index].y < vertices[closest].y)
            {
                src = (Rectangle){70, 81, 16, 38};
            }
        }
        else if (vertices[index].y == vertices[closest].y)
        {
            if (vertices[index].x > vertices[closest].x)
            {
                src = (Rectangle){1, 103, 49, 18};
            }
            else if (vertices[index].x < vertices[closest].x)
            {
                src = (Rectangle){1, 83, 49, 18};
            }
        }
        else
        {
            Point from = route[0];
            Point to = route[1];
            float dx = to.x - from.x;
            float dy = to.y - from.y;

            if (fabsf(dy) > fabsf(dx)) {
                // Mouvement plutôt vertical
                if (vertices[index].type == 0 && vertices[closest].type == 1)
                    src = (Rectangle){70, 81, 16, 38};  // bas
                else if (vertices[index].type == 0 && vertices[closest].type == 2)
                    src = (Rectangle){54, 121, 15, 38}; // bas
                else
                    src = (Rectangle){70, 81, 16, 38};  // fallback
            } else {
                // Mouvement plutôt horizontal
                if (vertices[index].type == 0 && vertices[closest].type == 1)
                    src = (Rectangle){1, 83, 49, 18};  // droite
                else if (vertices[index].type == 0 && vertices[closest].type == 2)
                    src = (Rectangle){6, 123, 46, 17}; // droite
                else
                    src = (Rectangle){1, 83, 49, 18};  // fallback
            }

            rotate = true;
        }
    }
    else if (vertices[index].type == 0 && vertices[closest].type == 2)
    {
        if (vertices[index].x == vertices[closest].x)
        {
            if (vertices[index].y > vertices[closest].y)
            {
                src = (Rectangle){71, 121, 15, 38};
            }
            else if (vertices[index].y < vertices[closest].y)
            {
                src = (Rectangle){54, 121, 15, 38};
            }
        }
        else if (vertices[index].y == vertices[closest].y)
        {
            if (vertices[index].x > vertices[closest].x)
            {
                src = (Rectangle){6, 142, 46, 17};
            }
            else if (vertices[index].x < vertices[closest].x)
            {
                src = (Rectangle){6, 123, 46, 17};
            }
        }
        else
        {
            Point from = route[0];
            Point to = route[1];
            float dx = to.x - from.x;
            float dy = to.y - from.y;

            if (fabsf(dy) > fabsf(dx)) {
                // Mouvement plutôt vertical
                if (vertices[index].type == 0 && vertices[closest].type == 1)
                    src = (Rectangle){70, 81, 16, 38};  // bas
                else if (vertices[index].type == 0 && vertices[closest].type == 2)
                    src = (Rectangle){54, 121, 15, 38}; // bas
                else
                    src = (Rectangle){70, 81, 16, 38};  // fallback
            } else {
                // Mouvement plutôt horizontal
                if (vertices[index].type == 0 && vertices[closest].type == 1)
                    src = (Rectangle){1, 83, 49, 18};  // droite
                else if (vertices[index].type == 0 && vertices[closest].type == 2)
                    src = (Rectangle){6, 123, 46, 17}; // droite
                else
                    src = (Rectangle){1, 83, 49, 18};  // fallback
            }

            rotate = true;
        }
    }
    
    draw_car_animation(route, point_count, reverse, src, voitures, rotate, elapsed, scale, offsetX, offsetY);

    if (elapsed >= 6.0f) {
        vertices[index].issue = vertices[index].need = 0;
        animation_started[index] = false;
    }
}

void draw_car_animation(Point route[], int point_count, bool reverse, Rectangle src, Texture2D voitures, bool rotate, 
    double elapsed, float scale, float offsetX, float offsetY) {
    float total_duration = 3.0f;
    float vehicle_scale = 1.0f;

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

        float x = a.x + f * (b.x - a.x);
        float y = a.y + f * (b.y - a.y);
        float angle = 0.0f;
        
        Rectangle dest = {
            x - src.width * vehicle_scale / 2,
            y - src.height * vehicle_scale / 2,
            src.width * vehicle_scale,
            src.height * vehicle_scale
        };

        if (rotate) {
            float dx = b.x - a.x;
            float dy = b.y - a.y;
            angle = atan2f(dy, dx) * (180.0f / PI);
        }

        DrawTexturePro(voitures, src, dest,
                       (rotate ? (Vector2){src.width * vehicle_scale / 2, src.height * vehicle_scale / 2} : (Vector2){0, 0}),
                       angle, WHITE);
    }
    else if (elapsed < 6.0f) {
        Point last = reverse ? route[0] : route[point_count - 1];
        Rectangle dest = {
            last.x - src.width * vehicle_scale / 2,
            last.y - src.height * vehicle_scale / 2,
            src.width * vehicle_scale,
            src.height * vehicle_scale
        };

        float angle = 0.0f;
        if (rotate) {
            Point before_last = reverse ? route[1] : route[point_count - 2];
            float dx = last.x - before_last.x;
            float dy = last.y - before_last.y;
            angle = atan2f(dy, dx) * (180.0f / PI);
        }

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