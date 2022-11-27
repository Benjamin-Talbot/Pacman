#include "mapFunctions.h"

pMap mapInit(pMap this, int rows, int cols, char* elems) {
    this->width = cols;
    this->height = rows;
    this->elems = elems;
    return this;
}

void drawMap(pMap this, char arr[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(arr[r][c] != 32) {
                mvprintw(r, c, "%c", arr[r][c]);
            }
        }
    }
}