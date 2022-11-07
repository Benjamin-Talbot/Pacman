#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

typedef struct Map {
    int width;
    int height;
    char* elems;
} Map;
typedef Map* pMap;

pMap mapInit(pMap this, int rows, int cols, char* elems);

char* loadMap(char* str, int rows, int cols, int level);

void drawMap(pMap this, char arr[this->height][this->width]);

#endif