#include "animation.h"

void build_path_asset(char *full_path, const char *filename) {
    const char *base_path = "City_Tilemap/";

    snprintf(full_path, PATH_MAX, "%s%s", base_path, filename);
    printf("Full path to file: %s\n", full_path);  // pour debug
}
