#ifndef GRAPHE_H
#define GRAPHE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int state;        // 0, 1, 2
    int traffic;      // 0 or 1
    float weight;     // weight of the road 
} Road;

typedef struct {
    int type;              // 0, 1, 2 (city, hospital, warehouse)
    int storage;           
    int storage_limit;     // maximum storage capacity
} Vertex; 

#endif