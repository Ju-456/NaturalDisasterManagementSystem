#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "graph.h"

int min(int n, Road matrix[][MAX_VERTICES], int j);
void dijkstra(int n, Road matrix[][MAX_VERTICES], int start);

#endif