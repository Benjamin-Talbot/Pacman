#include "mapFunctions.h"

// initialize the map struct
pMap mapInit(pMap this, int rows, int cols, char* elems) {
    this->width = cols;
    this->height = rows;
    this->elems = elems;
    return this;
}

// erase a pellet from the map's elements
void eatPellet(pMap this, char elems[this->height][this->width], int x, int y) {
    elems[y][x] = ' ';
}

// draw the walls of the map
void drawWalls(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(elems[r][c] == WALL || elems[r][c] == DOOR) {
                mvprintw(r, c, "%c", elems[r][c]);
            }
        }
    }
}

// draw the map's pellets and spaces
void drawMap(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(elems[r][c] == PELLET || elems[r][c] == SPACE) {
                mvprintw(r, c, "%c", elems[r][c]);
            }
        }
    }
}

// clear all the elements of the map
void clearMap(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            mvprintw(r, c, " ");
        }
    }
}