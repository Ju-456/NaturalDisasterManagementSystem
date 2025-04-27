#include "window.h"
#include "road.h"

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

bool are_connected_with_id(const char *id1, const char *id2, int num_roads, Road roads[], Vertex vertices[]) {
    for (int i = 0; i < num_roads; i++) {
        const char *start_id = vertices[roads[i].start].id;
        const char *end_id = vertices[roads[i].end].id;

        if ((strcmp(start_id, id1) == 0 && strcmp(end_id, id2) == 0) ||
            (strcmp(start_id, id2) == 0 && strcmp(end_id, id1) == 0)) {
                return i;
            }
        }
        return -1;
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

void draw_state_for_existing_roads(int num_vertices, Vertex vertices[], Road matrix[][100], Road roads[], int num_roads) {
    static bool first_call = true; // to prevent having orange without changing states
    int k = 0;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            Vertex *a = &vertices[i];
            Vertex *b = &vertices[j];

            if (!are_connected(a->id, b->id, num_roads, roads, vertices)) continue;

            // display only existing roads
            if (matrix[i][j].weight <= 0) continue;

            // int road_index = are_connected_with_id(a->id, b->id, num_roads, roads, vertices);
            // if (road_index == -1) continue;

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
    DrawText("Click outside to close", box_x + 10, box_y + 170, 14, DARKGRAY);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (!(mouse.x > box_x && mouse.x < box_x + box_w && mouse.y > box_y && mouse.y < box_y + box_h)) {
            *mode = MODE_GRAPH;
            *selected_index = -1;
        }
    }
}

void init_window_road(Vertex *original_vertices, Vertex *scaled_vertices,Road *roads, int num_roads,AppMode *mode, int *selected_index,Road matrix[][100]){
    if (*selected_index == -1)
        return;

    Road r = roads[*selected_index];
    int i = r.start, j = r.end;

    int box_x = 50, box_y = 50, box_w = 350, box_h = 180;
    DrawRectangle(box_x, box_y, box_w, box_h, LIGHTGRAY);
    DrawRectangleLines(box_x, box_y, box_w, box_h, DARKGRAY);

    DrawText(TextFormat("Road: %s -> %s",original_vertices[i].id, original_vertices[j].id),box_x + 10, box_y + 10, 20, BLACK);
    DrawText(TextFormat("Type: (%s -> %s)",get_type_name(original_vertices[i].type),get_type_name(original_vertices[j].type)),box_x + 10, box_y + 40, 20, BLACK);

    int old_state = r.state; // initial state
    int new_state = matrix[i][j].state; // currrent state
    if (old_state != new_state){
        DrawText(TextFormat("State: (%d -> %d)", old_state, new_state),box_x + 10, box_y + 70, 20, ORANGE);
    }
    else{
        DrawText(TextFormat("State: %d", new_state),box_x + 10, box_y + 70, 20, BLACK);
    }

    DrawText(TextFormat("Weight: %.0f", r.weight),box_x + 10, box_y + 100, 20, BLACK);
    DrawText(TextFormat("Capacity: %d %s", r.road_capacity,(r.road_capacity <= 1) ? "vehicle" : "vehicles"),box_x + 10, box_y + 130, 20, BLACK);

    DrawText("Click outside to close", box_x + 10, box_y + 160, 14, DARKGRAY);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        Vector2 m = GetMousePosition();
        if (m.x < box_x || m.x > box_x + box_w || m.y < box_y || m.y > box_y + box_h){
            *mode = MODE_GRAPH;
            *selected_index = -1;
        }
    }
}

void transition_window(Texture2D transition_texture, Texture2D grass_texture, const char *message) {
    float elapsed = 0.0f;

    // during 4 sec
    while (elapsed < 4.0f && !WindowShouldClose()) {
        float dt = GetFrameTime();
        elapsed += dt;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // same graph display
        for (int x = 0; x < GetScreenWidth(); x += grass_texture.width) {
            for (int y = 0; y < GetScreenHeight(); y += grass_texture.height) {
                DrawTexture(grass_texture, x, y, WHITE);
            }
        }

        // (fade effect)
        unsigned char alpha = (unsigned char)(255 * (1.0f - elapsed / 4.0f));
        Color fade_color = (Color){255, 255, 255, alpha};

        // center the picture
        int img_x = (GetScreenWidth() - transition_texture.width) / 2;
        int img_y = (GetScreenHeight() - transition_texture.height) / 2;
        DrawTexture(transition_texture, img_x, img_y, fade_color);

        // center the text picture IF THE MESSAGE ISN'T NULL !
        if (message != NULL) {
            int font_size = 40;
            int text_width = MeasureText(message, font_size);
            int text_x = img_x + (transition_texture.width - text_width) / 2;
            int text_y = img_y + transition_texture.height / 2 - font_size / 2;
            DrawText(message, text_x, text_y, font_size, fade_color);
        }
        EndDrawing();
    }
}

void button_click(bool *menu_open, bool *show_states, int num_vertices, Vertex *vertices, Road roads[], int num_roads,Texture2D transition_texture, Texture2D grass_texture, const char *message, Road matrix[][100]) {
    Rectangle menu_button = { 10, 10, 30, 20 };
    static double timer = 0;

    if (CheckCollisionPointRec(GetMousePosition(), menu_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *menu_open = !(*menu_open);
        timer = GetTime(); 
    }

    if (*menu_open && GetTime() - timer > 3.0) {
        Rectangle menu_rect = { menu_button.x, menu_button.y + 25, 140, 70 };
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            !CheckCollisionPointRec(GetMousePosition(), menu_rect)) {
            *menu_open = false;
        }
    }

    DrawCircle(menu_button.x + 1, menu_button.y + 5, 2, BLACK);
    DrawCircle(menu_button.x + 1, menu_button.y + 10, 2, BLACK);
    DrawCircle(menu_button.x + 1, menu_button.y + 15, 2, BLACK);

    if (*menu_open) {
        Rectangle menu_rect = { menu_button.x, menu_button.y + 25, 140, 70 };
        DrawRectangleRec(menu_rect, LIGHTGRAY);
        DrawText("earthquake", menu_rect.x + 25, menu_rect.y + 10, 12, BLACK);
        DrawText("state's roads", menu_rect.x + 25, menu_rect.y + 40, 12, BLACK);

        Rectangle checkbox1 = { menu_rect.x + 5, menu_rect.y + 10, 14, 14 };
        Rectangle checkbox2 = { menu_rect.x + 5, menu_rect.y + 40, 14, 14 };

        if (CheckCollisionPointRec(GetMousePosition(), checkbox1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            transition_window(transition_texture, grass_texture, "Be carfule !\nThe earthquake is near...");
            earthquake(num_vertices, matrix);
            /*for(int i = 0; i<num_roads; i++){
                printf("%d ", roads[i].state);
            }
            printf("\n");*/
        }

        if (CheckCollisionPointRec(GetMousePosition(), checkbox2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            draw_state_for_existing_roads(num_vertices, vertices, matrix, roads, num_roads);
            *show_states = !(*show_states);
        }

        DrawRectangleRec(checkbox1, RAYWHITE);
        DrawRectangleRec(checkbox2, RAYWHITE);

        DrawText("X", checkbox1.x + 2, checkbox1.y - 2, 14, GREEN);

        if (*show_states) {
            DrawText("X", checkbox2.x + 2, checkbox2.y - 2, 14, GREEN);
        }
    }
}

void init_window_custom(const char *filename, int num_vertices, Vertex *vertices, Road *roads, int num_roads, Road matrix[][100]) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Map of Graph 1 :");
    SetTargetFPS(60);

    road_texture = LoadTexture("City_Tilemap/City_Transparent.png");
    Texture2D grass_texture = LoadTexture("City_Tilemap/grass_retro1.png");
    Texture2D transition_texture = LoadTexture("City_Tilemap/transition_texture.png");
    AppMode mode = MODE_GRAPH;
    int selected_index = -1;
    bool menu_open = false;
    bool show_states = false;

    if (road_texture.id == 0) {
            printf("Route texture loading error.\n");
            return;
    }

    if (grass_texture.id == 0) {
        printf("Background texture loading error.\n");
        return;
    }

    if (transition_texture.id == 0 || grass_texture.id == 0) {
        printf("Erreur de chargement de texture.\n");
        return;
    }

    // transition_window(transition_texture, grass_texture, "This is the graph \nbefore the earthquake..."); 
    load_graph_from_json(filename, &num_vertices, vertices, roads, &num_roads);

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
        button_click(&menu_open, &show_states, num_vertices, vertices, roads, num_roads, transition_texture, grass_texture, NULL, matrix);

        if (show_states) {
            draw_state_for_existing_roads(num_vertices, vertices, matrix, roads, num_roads);
        }

        // the user didn't click on a vertex or road
        if (mode == MODE_GRAPH) {
            Vector2 mouse = GetMousePosition();
        
            // Right click for vertices
            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                for (int i = 0; i < num_vertices; i++) {
                    float dx = mouse.x - scaled_vertices[i].x;
                    float dy = mouse.y - scaled_vertices[i].y;
                    float distance = sqrtf(dx * dx + dy * dy);
                    if (distance <= 50.0f) {
                        selected_index = i;
                        mode = MODE_VERTEX_DETAILS;
                    }
                }
            }
        
            // Left click for roads
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int r = 0; r < num_roads; r++) {
                    Vector2 p1 = {scaled_vertices[roads[r].start].x, scaled_vertices[roads[r].start].y};
                    Vector2 p2 = {scaled_vertices[roads[r].end].x, scaled_vertices[roads[r].end].y};
        
                    float dx = p2.x - p1.x;
                    float dy = p2.y - p1.y;
                    float len_sq = dx * dx + dy * dy;
                    float t = ((mouse.x - p1.x) * dx + (mouse.y - p1.y) * dy) / len_sq;
                    t = fmaxf(0, fminf(1, t));
                    float proj_x = p1.x + t * dx;
                    float proj_y = p1.y + t * dy;
        
                    float dist = sqrtf((mouse.x - proj_x) * (mouse.x - proj_x) + (mouse.y - proj_y) * (mouse.y - proj_y));
                    if (dist < 12.0f && len_sq > 1000.0f) {
                        selected_index = r;
                        mode = MODE_ROAD_DETAILS;
                    }
                }
            }
        }
        else if (mode == MODE_VERTEX_DETAILS && selected_index != -1) {
            init_window_vertex(vertices, scaled_vertices, num_vertices, &mode, &selected_index);
        }
        else if (mode == MODE_ROAD_DETAILS && selected_index != -1) {
            init_window_road(vertices, scaled_vertices, roads, num_roads, &mode, &selected_index, matrix);
        }

        EndDrawing();
    }

    UnloadTexture(road_texture);
    CloseWindow();
}
