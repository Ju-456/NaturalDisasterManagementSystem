#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  
#include <limits.h> 

#ifndef PATH_MAX
#define PATH_MAX 4096 
#endif

// Assets gestion's part
void build_path_asset(char *full_path, const char *filename);

#endif