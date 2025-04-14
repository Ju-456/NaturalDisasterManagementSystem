#ifndef GRAPHE_H
#define GRAPHE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int state;        // 0, 1, 2
    float weight;     
    int road_capacity; // depends of the weight
} Road;

typedef struct {
    int type;              // 0, 1, 2 (city, hospital, warehouse)
    int storage;           
    int storage_capacity;     // maximum storage capacity
} Vertex; 

typedef struct {
    int ambulance;
    int truck;
    int vehicle_capacity;
} Vehicle;

#endif