#include "general.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define FILENAME "graph1.json"

void build_path(char *full_path) {
    const char *home = getenv("HOME");
    const char *base_path = "/GraphVisualizer/Graph_json/";
    if (home != NULL) {
        snprintf(full_path, PATH_MAX, "%s%s%s", home, base_path, FILENAME);
        printf("Full path: %s\n", full_path);
    } else {
        fprintf(stderr, "Could not determine home directory\n");
    }
}