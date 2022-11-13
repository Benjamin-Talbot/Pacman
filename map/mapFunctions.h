#ifndef MAP_FUNCTIONS_H
#define MAP_FUNCTIONS_H

#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

pMap mapInit(pMap this, int rows, int cols, char* elems);

char* loadMap(char* str, int rows, int cols, int level);

void drawMap(pMap this, char arr[this->height][this->width]);

#endif