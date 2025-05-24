#include "window_general_part.h"

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

void init_window_vertex(Vertex *vertices, Vertex *scaled_vertices, int num_vertices, AppMode *mode, int *selected_index) {
    if (*selected_index == -1) return;

    Vertex v = vertices[*selected_index];
    int box_x = 50, box_y = 50, box_w = 300, box_h = 150;

    DrawRectangle(box_x, box_y, box_w, box_h, LIGHTGRAY);
    DrawRectangleLines(box_x, box_y, box_w, box_h, DARKGRAY);

    DrawText(TextFormat("ID: %s", v.id), box_x + 10, box_y + 10, 20, BLACK);

    const char* type_str = (v.type == 0) ? "City" : (v.type == 1) ? "Hospital" : "Warehouse";
    DrawText(TextFormat("Type: %s", type_str), box_x + 10, box_y + 40, 20, BLACK);
    DrawText(TextFormat("Degree: %d", v.degree), box_x + 10, box_y + 70, 20, BLACK);
    DrawText(TextFormat("Capacity: %d %s", v.storage_capacity, (v.storage_capacity == 1 || v.storage_capacity == 0) ? "kg" : "kgs"), box_x + 10, box_y + 100, 20, BLACK);
    DrawText("Click outside to close", box_x + 10, box_y + 130, 14, DARKGRAY);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (!(mouse.x > box_x && mouse.x < box_x + box_w && mouse.y > box_y && mouse.y < box_y + box_h)) {
            *mode = MODE_GRAPH;
            *selected_index = -1;
        }
    }
}

void init_window_road(Vertex *original_vertices, Vertex *scaled_vertices,Road *roads, int num_roads,AppMode *mode, int *selected_index,Road matrix[][MAX_VERTICES]){
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

    DrawText(TextFormat("Weight: %.0f", r.weight),box_x + 10, box_y + MAX_VERTICES, 20, BLACK);
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

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    float scale = fminf((float)screenWidth / transition_texture.width,
                        (float)screenHeight / transition_texture.height);

    int scaled_width = transition_texture.width * scale;
    int scaled_height = transition_texture.height * scale;

    int img_x = (screenWidth - scaled_width) / 2;
    int img_y = (screenHeight - scaled_height) / 2;

    while (elapsed < 4.0f && !WindowShouldClose()) {
        float dt = GetFrameTime();
        elapsed += dt;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int x = 0; x < screenWidth; x += grass_texture.width) {
            for (int y = 0; y < screenHeight; y += grass_texture.height) {
                DrawTexture(grass_texture, x, y, WHITE);
            }
        }

        unsigned char alpha = (unsigned char)(255 * (1.0f - elapsed / 4.0f));
        Color fade_color = (Color){255, 255, 255, alpha};

        Rectangle src = {0, 0, (float)transition_texture.width, (float)transition_texture.height};
        Rectangle dest = {img_x, img_y, (float)scaled_width, (float)scaled_height};
        Vector2 origin = {0, 0};
        DrawTexturePro(transition_texture, src, dest, origin, 0.0f, fade_color);

        if (message != NULL) {
            int font_size = scaled_height / 10;  
            int text_width = MeasureText(message, font_size);
            int text_x = img_x + (scaled_width - text_width) / 2;
            int text_y = img_y + scaled_height / 2 - font_size / 2;
            DrawText(message, text_x, text_y, font_size, fade_color);
        }

        EndDrawing();
    }
}

void button_click(bool *menu_open, bool *show_states, int num_vertices, Vertex *vertices, Road roads[], int num_roads, 
    bool *show_group_vertices, Texture2D transition_texture, Texture2D grass_texture, const char *message, 
    Road matrix[][MAX_VERTICES], int order_for_intervention) {
    
    Rectangle menu_button = { 10, 10, 30, 20 };
    static double timer = 0;
    static int current_intervention_index = 0; // to treat issue step by step
    static bool interventions_initialized = false;
    static bool group_display_initialized = false;

    if (CheckCollisionPointRec(GetMousePosition(), menu_button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *menu_open = !(*menu_open);
        timer = GetTime(); 
    }

    if (*menu_open && GetTime() - timer > 3.0) {
        Rectangle menu_rect = { menu_button.x, menu_button.y + 25, 140, 95 };
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            !CheckCollisionPointRec(GetMousePosition(), menu_rect)) {
            *menu_open = false;
        }
    }

    DrawCircle(menu_button.x + 1, menu_button.y + 5, 2, BLACK);
    DrawCircle(menu_button.x + 1, menu_button.y + 10, 2, BLACK);
    DrawCircle(menu_button.x + 1, menu_button.y + 15, 2, BLACK);

    if (*menu_open) { // un button = 30 pixels of lenght and 
        Rectangle menu_rect = { menu_button.x, menu_button.y + 25, 165, 160 };
        DrawRectangleRec(menu_rect, LIGHTGRAY);
        DrawText("Earthquake", menu_rect.x + 25, menu_rect.y + 10, 12, BLACK);
        DrawText("State's roads", menu_rect.x + 25, menu_rect.y + 40, 12, BLACK);
        DrawText("Intervention", menu_rect.x + 25, menu_rect.y + 70, 12, BLACK);
        DrawText("Group Accessible Areas", menu_rect.x + 25, menu_rect.y + 100, 12, BLACK);
        DrawText("Mission n*4 ", menu_rect.x + 25, menu_rect.y + 130, 12, BLACK); // to change 

        Rectangle checkbox1 = { menu_rect.x + 5, menu_rect.y + 10, 14, 14 };
        Rectangle checkbox2 = { menu_rect.x + 5, menu_rect.y + 40, 14, 14 };
        Rectangle checkbox3 = { menu_rect.x + 5, menu_rect.y + 70, 14, 14 };
        Rectangle checkbox4 = { menu_rect.x + 5, menu_rect.y + 100, 14, 14 };
        Rectangle checkbox5 = { menu_rect.x + 5, menu_rect.y + 130, 14, 14 };

        // Earthquake
        if (CheckCollisionPointRec(GetMousePosition(), checkbox1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            transition_window(transition_texture, grass_texture, "Be careful!\nThe earthquake is near...");
            earthquake(num_vertices, matrix, num_roads);
        }

        // Show state's roads
        if (CheckCollisionPointRec(GetMousePosition(), checkbox2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            draw_state_for_existing_roads(num_vertices, vertices, matrix, roads, num_roads);
            *show_states = !(*show_states);
        }

        // Start interventions
        if (CheckCollisionPointRec(GetMousePosition(), checkbox3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            transition_window(transition_texture, grass_texture, "Interventions will be \nmade in the order\nof priority established");
            init_city_need(num_vertices, matrix, vertices);
            init_type_of_issue(num_roads, matrix, vertices);        
            init_travel_time(num_vertices, matrix);    
                         
            display_info_travel(num_vertices, matrix, vertices, roads, num_roads);   
            travel_to_city(num_vertices, matrix, vertices, &order_for_intervention);
            update_closest_vertices(num_vertices, vertices);

            current_intervention_index = 0;
            interventions_initialized = true;
        }

        // Group Accessible Areas
        if (CheckCollisionPointRec(GetMousePosition(), checkbox4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {  
        *show_group_vertices = !(*show_group_vertices);

        if (!group_display_initialized) {
            //transition_window(transition_texture, grass_texture, "These are\nGroup Accessible Areas");

            int finishing_order[MAX_VERTICES];
            int pos = 0;
            left_right_root_full(num_vertices, matrix, finishing_order, &pos);

            Road matrix_inverse[MAX_VERTICES][MAX_VERTICES];
            inverse_matrix_2d_full(num_vertices, matrix, matrix_inverse);

            depth_first_search_inverse(num_vertices, matrix, matrix_inverse, finishing_order);

            group_display_initialized = true;
        }
    }
        
        // Show Group Accessible Areas
        if (*show_group_vertices) {
            draw_group_of_vertices(num_vertices, matrix, vertices);
        }

        DrawRectangleRec(checkbox1, RAYWHITE);
        DrawRectangleRec(checkbox2, RAYWHITE);
        DrawRectangleRec(checkbox3, RAYWHITE);
        DrawRectangleRec(checkbox4, RAYWHITE);
        DrawRectangleRec(checkbox5, RAYWHITE);

        if (*show_states) DrawText("X", checkbox2.x + 2, checkbox2.y - 2, 14, GREEN);
        if (*show_group_vertices) DrawText("X", checkbox4.x + 2, checkbox4.y - 2, 14, GREEN);
    }

    if (interventions_initialized) {
        while (current_intervention_index < num_vertices) {
            if (vertices[current_intervention_index].need == 1 && vertices[current_intervention_index].closest != -1) {
                draw_travel_effects(num_vertices, vertices, current_intervention_index);
                break; // only one display per frame
            }
            current_intervention_index++;
        }

        if (current_intervention_index >= num_vertices) {
            interventions_initialized = false;
        }
    }
}

void init_window_custom(const char *filename, int num_vertices, Vertex *vertices, Road *roads, 
    int num_roads, Road matrix[][MAX_VERTICES], int order_for_intervention) {
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Map of Graph 1 :");
    SetTargetFPS(60);

    road_texture = LoadTexture("Assets/City_Transparent.png");
    Texture2D grass_texture = LoadTexture("Assets/grass_retro1.png");
    Texture2D transition_texture = LoadTexture("Assets/transition_texture.png");
    AppMode mode = MODE_GRAPH;
    int selected_index = -1;
    bool menu_open = false;
    bool show_states = false;
    bool show_group_vertices = false;

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
        button_click(&menu_open, &show_states, num_vertices, vertices, roads, num_roads, &show_group_vertices, transition_texture, 
            grass_texture, NULL, matrix, order_for_intervention);

        if (show_states) {
            draw_state_for_existing_roads(num_vertices, scaled_vertices, matrix, roads, num_roads);
        }

        if (show_group_vertices) {
        draw_group_of_vertices(num_vertices, matrix, vertices);
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
