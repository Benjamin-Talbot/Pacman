#include "mapFunctions.h"

#ifndef WALL
#define WALL '#'    // 35
#endif
#ifndef DOOR
#define DOOR '-'    // 35
#endif
#ifndef PELLET
#define PELLET '.'   // 46
#endif

pMap mapInit(pMap this, int rows, int cols, char* elems) {
    this->width = cols;
    this->height = rows;
    this->elems = elems;
    return this;
}

void drawWalls(pMap this, char arr[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(arr[r][c] == WALL || arr[r][c] == DOOR) {
                mvprintw(r, c, "%c", arr[r][c]);
            }
        }
    }
}

void drawMap(pMap this, char arr[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(arr[r][c] == PELLET) {
                mvprintw(r, c, "%c", arr[r][c]);
            }
        }
    }
}