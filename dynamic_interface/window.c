#include "graph.h"

Texture2D road_texture;

Vertex *find_vertex_by_id(const char *id, int num_vertices, Vertex vertices[]) {
    for (int i = 0; i < num_vertices; i++) {
        if (strcmp(vertices[i].id, id) == 0) return &vertices[i];
    }
    return NULL;
}

bool are_connected(const char *id1, const char *id2, int num_roads, Road roads[], Vertex vertices[]) {
    for (int i = 0; i < num_roads; i++) {
        const char *start_id = vertices[roads[i].start].id;
        const char *end_id = vertices[roads[i].end].id;

        if ((strcmp(start_id, id1) == 0 && strcmp(end_id, id2) == 0) ||
            (strcmp(start_id, id2) == 0 && strcmp(end_id, id1) == 0)) {
            return true;
        }
    }
    return false;
}

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

void init_window_vertex(Vertex *vertices, Vertex *scaled_vertices, int num_vertices, AppMode *mode, int *selected_index) {
    if (*selected_index == -1) return;

    Vertex v = vertices[*selected_index];
    int box_x = 50, box_y = 50, box_w = 300, box_h = 200;

    DrawRectangle(box_x, box_y, box_w, box_h, LIGHTGRAY);
    DrawRectangleLines(box_x, box_y, box_w, box_h, DARKGRAY);

    DrawText(TextFormat("ID: %s", v.id), box_x + 10, box_y + 10, 20, BLACK);

    const char* type_str = (v.type == 0) ? "City" : (v.type == 1) ? "Hospital" : "Warehouse";
    DrawText(TextFormat("Type: %s", type_str), box_x + 10, box_y + 40, 20, BLACK);
    DrawText(TextFormat("Degree: %d", v.degree), box_x + 10, box_y + 70, 20, BLACK);

    const char* storage_str =
        (v.type == 1) ? ((v.type_of_storage == 0) ? "Food, Meds" : "Meds, Food") :
        (v.type == 0) ? ((v.type_of_storage == 1) ? "Meds, Other" : "Other, Meds") :
        "Other";

    DrawText(TextFormat("Storage: %s", storage_str), box_x + 10, box_y + 100, 20, BLACK);
    DrawText(TextFormat("Capacity: %d %s", v.storage_capacity, (v.storage_capacity == 1 || v.storage_capacity == 0) ? "kg" : "kgs"), box_x + 10, box_y + 130, 20, BLACK);
    // DrawText(TextFormat("Capacity: %d kgs", v.storage_capacity), box_x + 10, box_y + 130, 20, BLACK);
    DrawText("Click outside to close", box_x + 10, box_y + 170, 14, DARKGRAY);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (!(mouse.x > box_x && mouse.x < box_x + box_w && mouse.y > box_y && mouse.y < box_y + box_h)) {
            *mode = MODE_GRAPH;
            *selected_index = -1;
        }
    }
}

void init_window_road(Vertex *original_vertices, Vertex *scaled_vertices, Road *roads, int num_roads, AppMode *mode, int *selected_index) {
    if (*selected_index == -1) return;

    Road r = roads[*selected_index];
    int box_x = 50, box_y = 50, box_w = 350, box_h = 180;

    DrawRectangle(box_x, box_y, box_w, box_h, LIGHTGRAY);
    DrawRectangleLines(box_x, box_y, box_w, box_h, DARKGRAY);

    DrawText(TextFormat("Road: %s -> %s", original_vertices[r.start].id, original_vertices[r.end].id), box_x + 10, box_y + 10, 20, BLACK);
    DrawText(TextFormat("Type: (%s -> %s)", get_type_name(original_vertices[r.start].type), get_type_name(original_vertices[r.end].type)), box_x + 10, box_y + 40, 20, BLACK);
    DrawText(TextFormat("State: %d", r.state), box_x + 10, box_y + 70, 20, BLACK);
    DrawText(TextFormat("Weight: %.0f", r.weight), box_x + 10, box_y + 100, 20, BLACK);
    DrawText(TextFormat("Capacity: %d %s", r.road_capacity, (r.road_capacity == 1 || r.road_capacity == 0) ? "vehicle" : "vehicles"), box_x + 10, box_y + 130, 20, BLACK);
    DrawText("Click outside to close", box_x + 10, box_y + 160, 14, DARKGRAY);

    // close the window if the user click outside
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (!(mouse.x > box_x && mouse.x < box_x + box_w && mouse.y > box_y && mouse.y < box_y + box_h)) {
            *mode = MODE_GRAPH;
            *selected_index = -1;
        }
    }
}

void init_window_custom(const char *filename, int num_vertices, Vertex *vertices, Road *roads, int num_roads) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Map of Graph 1 :");
    SetTargetFPS(60);

    road_texture = LoadTexture("City_Tilemap/City_Transparent.png");
    if (road_texture.id == 0) {
        printf("Route texture loading error.\n");
        return;
    }

    Texture2D grass_texture = LoadTexture("City_Tilemap/grass_texture.jpg");
    if (grass_texture.id == 0) {
        printf("Background texture loading error.\n");
        return;
    }

    load_graph_from_json(filename, &num_vertices, vertices, roads, &num_roads);

    AppMode mode = MODE_GRAPH;
    int selected_index = -1;

    while (!WindowShouldClose()) {
        int screen_width = GetScreenWidth();
        int screen_height = GetScreenHeight();

        // Scaling isotropique pour garder les proportions correctes (surtout diagonales)
        float scale = fminf((float)screen_width / SCREEN_WIDTH, (float)screen_height / SCREEN_HEIGHT);

        // Centred the graph in the window
        float offsetX = (screen_width - SCREEN_WIDTH * scale) / 2.0f;
        float offsetY = (screen_height - SCREEN_HEIGHT * scale) / 2.0f;

        // temporary copy with adapted values
        Vertex scaled_vertices[num_vertices];
        for (int i = 0; i < num_vertices; i++) {
            scaled_vertices[i] = vertices[i];
            scaled_vertices[i].x = vertices[i].x * scale + offsetX;
            scaled_vertices[i].y = vertices[i].y * scale + offsetY;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // the adapted display
        for (int x = 0; x < screen_width; x += grass_texture.width) {
            for (int y = 0; y < screen_height; y += grass_texture.height) {
                DrawTexture(grass_texture, x, y, WHITE);
            }
        }
        
        // To adapt the size if the user click "full screen"
        draw_roads_with_orientation(num_vertices, scaled_vertices, roads, num_roads);
        draw_vertices_with_type(num_vertices, scaled_vertices);

        // the user didn't click on a vertex or road
        if (mode == MODE_GRAPH) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
        
                // Check vertices first
                for (int i = 0; i < num_vertices; i++) {
                    float dx = mouse.x - scaled_vertices[i].x;
                    float dy = mouse.y - scaled_vertices[i].y;
                    float distance = sqrtf(dx * dx + dy * dy);
                    if (distance <= 50.0f) {
                        selected_index = i;
                        mode = MODE_VERTEX_DETAILS;
                    }
                }
        
                // Then check roads
                for (int r = 0; r < num_roads; r++) {
                    Vector2 p1 = {scaled_vertices[roads[r].start].x, scaled_vertices[roads[r].start].y};
                    Vector2 p2 = {scaled_vertices[roads[r].end].x, scaled_vertices[roads[r].end].y};
                    Vector2 m = GetMousePosition();
        
                    // Projection d'un point sur un segment
                    float dx = p2.x - p1.x;
                    float dy = p2.y - p1.y;
                    float len_sq = dx * dx + dy * dy;
                    float t = ((m.x - p1.x) * dx + (m.y - p1.y) * dy) / len_sq;
                    t = fmaxf(0, fminf(1, t)); // clamp t
                    float proj_x = p1.x + t * dx;
                    float proj_y = p1.y + t * dy;
        
                    float dist = sqrtf((m.x - proj_x) * (m.x - proj_x) + (m.y - proj_y) * (m.y - proj_y));
                    if (dist < 12.0f && len_sq > 1000.0f) { // the tolerance is bigger for the lengh than the width, bc there're roads
                        selected_index = r;
                        mode = MODE_ROAD_DETAILS;
                    }
                }
            }
        } 
        else if (mode == MODE_ROAD_DETAILS && selected_index != -1) {
            init_window_road(vertices, scaled_vertices, roads, num_roads, &mode, &selected_index);
        } 
        else if (mode == MODE_VERTEX_DETAILS && selected_index != -1) {
            init_window_vertex(vertices, scaled_vertices, num_vertices, &mode, &selected_index);
        }     
                
        EndDrawing();
    }

    UnloadTexture(road_texture);
    CloseWindow();
}