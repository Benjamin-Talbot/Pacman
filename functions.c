#include "functions.h"

char getInput(clock_t start, int updateRate) {
    char nextc = getch();
    char c;

    clock_t end;
    long time_elapsed;
    end = clock();
    time_elapsed = (long) ((end - start)/(double)CLOCKS_PER_SEC*1000);

    while(nextc != ERR) {
        c = nextc;
        nextc = getch();
    }

    while(time_elapsed < updateRate) {
        nextc = getch();
        if(nextc != -1) {
            c = nextc;
        }
        end = clock();
        time_elapsed = (long) ((end - start)/(double)CLOCKS_PER_SEC*1000);
    }

    return c;
}

void update(pPacman pacman, char c, pMap map) {
    pacmanMove(pacman, c, map, map->elems);    // move pacman
}

void draw(pPacman pacman, pMap map) {
    pacmanDraw(pacman);    // print pacman
    drawMap(map, map->elems);    // draw the board
}