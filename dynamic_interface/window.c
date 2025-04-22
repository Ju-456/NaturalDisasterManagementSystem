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

        DrawText(vertex->id, pos.x + 5, pos.y - 10, 10, BLACK); // to see the other vertices who still don't have building
    }
}

// to adapt the screen's size
void init_window_custom(const char *filename, int num_vertices, Vertex *vertices, Road *roads, int num_roads) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph Visualizer");
    SetTargetFPS(60);

    road_texture = LoadTexture("City_Tilemap/City_Transparent.png");
    if (road_texture.id == 0) {
        printf("Erreur de chargement de la texture de route.\n");
        return;
    }

    Texture2D grass_texture = LoadTexture("City_Tilemap/grass_texture.jpg");
    if (grass_texture.id == 0) {
        printf("Erreur de chargement de la texture de fond.\n");
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

        EndDrawing();
    }

    UnloadTexture(road_texture);
    CloseWindow();
}

// void init_vertex_window()