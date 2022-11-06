#include "map.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

pMap mapInit(pMap this, int rows, int cols, char* elems) {
    this->width = cols;
    this->height = rows;
    this->elems = elems;
    return this;
}

char* loadMap(char* dest, int rows, int cols, int level) {
    char* str = dest;

    int maxchars = sizeof(char)*(7+(int)log10(level) + 1) + 1;
    char filename[maxchars];
    snprintf(filename, maxchars, "map%d.txt", level);

    FILE* mapFile = fopen(filename, "r");
    char tmp[cols];
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            fscanf(mapFile, "%c", tmp);
            strcat(str, tmp);
        }
        fscanf(mapFile, "%c", tmp);
    }
    fclose(mapFile);

    return str;
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