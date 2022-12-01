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

void eatPellet(pMap this, char elems[this->height][this->width], int x, int y) {
    elems[y][x] = ' ';
}

void drawWalls(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(elems[r][c] == WALL || elems[r][c] == DOOR) {
                mvprintw(r, c, "%c", elems[r][c]);
            }
        }
    }
}

void drawMap(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(elems[r][c] == PELLET) {
                mvprintw(r, c, "%c", elems[r][c]);
            }
        }
    }
}

void clearMap(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            mvprintw(r, c, " ");
        }
    }
}