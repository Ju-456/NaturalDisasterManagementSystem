#include "vertex.h"
#include <string.h>
#include <stdio.h>

void generate_vertex_id(int index, char* id_out) {
    const int base = 52;
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    char temp[10];
    int len = 0;
    index++;
    while (index > 0) {
        index--;
        temp[len++] = charset[index % base];
        index /= base;
    }
    for (int i = 0; i < len; i++) {
        id_out[i] = temp[len - i - 1];
    }
    id_out[len] = '\0';
}