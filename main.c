#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include "pacman.h"
#include "map.h"
#include "functions.h"

// implement look ahead movement
// get map to offer width, height instead of rows, cols

int main() {
    clock_t start;
    int updateRate = 200;
    char c;
    const int rows = 15, cols = 40;
    char* elems = malloc(sizeof(char) * rows * cols);
    elems = loadMap(elems, rows, cols, 1);
    
    pMap map = (pMap) malloc(sizeof(Map));
    mapInit(map, rows, cols, elems);
    pPacman pacman = (pPacman) malloc(sizeof(Pacman));
    pacmanInit(pacman);

    initscr(); // creates stdscr
    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();


    while(1) {
        start = clock();

        update(pacman, c, map);
        draw(pacman, map);
        
        c = getInput(start, updateRate);
    }
    

    endwin();

    return 0;
}