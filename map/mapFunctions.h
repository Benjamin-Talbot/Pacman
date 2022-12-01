#ifndef MAP_FUNCTIONS_H
#define MAP_FUNCTIONS_H

#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

pMap mapInit(pMap this, int rows, int cols, char* elems);

void eatPellet(pMap this, char elems[this->height][this->width], int x, int y);

void drawWalls(pMap this, char elems[this->height][this->width]);

void drawMap(pMap this, char elems[this->height][this->width]);

void clearMap(pMap this, char elems[this->height][this->width]);

#endif