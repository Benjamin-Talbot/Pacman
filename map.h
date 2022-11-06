#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

typedef struct {
    int xwidth;
    int ywidth;
    char* elems;
} Map;
typedef Map* pMap;

pMap mapInit(pMap this, int rows, int cols, char* elems);

char* loadMap(char* str, int rows, int cols, int level);

void drawMap(int rows, int cols, char arr[rows][cols]);

#endif