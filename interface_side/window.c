#include "window.h"
#include "vertex.h"
#include "general.h"
#include "parson.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TOLERANCE 10.0f

typedef struct
{
    char id[3];
    float x;
    float y;
} Node;

typedef struct
{
    char from[3];
    char to[3];
    float weight;
} Edge;

static Node nodes[100];
static int node_count = 0;
static Edge edges[200];
static int edge_count = 0;

Texture2D road_texture; 

void LoadGraphFromJson(const char *filename) {
    char full_path[PATH_MAX];
    build_path(full_path);

    JSON_Value *root_val = json_parse_file(full_path);
    JSON_Object *root_obj = json_value_get_object(root_val);

    JSON_Array *ids = json_object_get_array(root_obj, "nodesIds");
    node_count = json_array_get_count(ids);

    for (int i = 0; i < node_count; i++) {
        const char *id = json_array_get_string(ids, i);
        JSON_Object *node = json_object_get_object(root_obj, id);

        strcpy(nodes[i].id, id);
        nodes[i].x = (float)json_object_get_number(node, "x");
        nodes[i].y = (float)json_object_get_number(node, "y");

        JSON_Array *adj = json_object_get_array(node, "adjencyList");
        for (int j = 0; j < json_array_get_count(adj); j++) {
            JSON_Array *conn = json_array_get_array(adj, j);
            const char *to = json_array_get_string(conn, 0);
            double weight = json_array_get_number(conn, 1);

            strcpy(edges[edge_count].from, id);
            strcpy(edges[edge_count].to, to);
            edges[edge_count].weight = weight;
            edge_count++;
        }
    }

    json_value_free(root_val);
}

Node *FindNodeById(const char *id) {
    for (int i = 0; i < node_count; i++) {
        if (strcmp(nodes[i].id, id) == 0) return &nodes[i];
    }
    return NULL;
}

bool AreConnected(const char *id1, const char *id2) {
    for (int i = 0; i < edge_count; i++) {
        if ((strcmp(edges[i].from, id1) == 0 && strcmp(edges[i].to, id2) == 0) ||
            (strcmp(edges[i].from, id2) == 0 && strcmp(edges[i].to, id1) == 0)) {
            return true;
        }
    }
    return false;
}

void DrawStraightRoads() {
    for (int i = 0; i < node_count; i++) {
        for (int j = i + 1; j < node_count; j++) {
            Node *a = &nodes[i];
            Node *b = &nodes[j];

            if (!AreConnected(a->id, b->id)) continue;

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

void InitWindowCustom() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graph Visualizer");
    SetTargetFPS(60);

    road_texture = LoadTexture("City_Tilemap/City_Transparent.png");
    if (road_texture.id == 0) {
        printf("Erreur de chargement de la texture de route.\n");
        return;
    }

    LoadGraphFromJson("dummy");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){0x37, 0x94, 0x6e, 255});

        DrawStraightRoads();

        for (int i = 0; i < node_count; i++) {
            DrawCircle((int)nodes[i].x, (int)nodes[i].y, 10, DARKBLUE);
            DrawText(nodes[i].id, nodes[i].x + 5, nodes[i].y - 5, 10, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(road_texture);
    CloseWindow();
}
