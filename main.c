#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include "pacman.h"
#include "map.h"

// implement look ahead movement

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

int main() {

    char keypressed = 0;
    clock_t start;
    int updateRate = 200;

    initscr(); // creates stdscr
    // timeout(updateRate);


    const int rows = 15, cols = 40;
    char* elems = malloc(sizeof(char) * rows * cols);
    elems = loadMap(elems, rows, cols, 1);
    
    pMap map = (pMap) malloc(sizeof(Map));
    map->xwidth = cols;
    map->ywidth = rows;
    // map->walls = elems;
    pPacman pacman = (pPacman) malloc(sizeof(Pacman));
    pacmanInit(pacman);

    int c;
    
    drawMap(rows, cols, elems);
    pacmanPrint(pacman);

    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();

    while(1) {
        start = clock();

        // update();
        // draw();

        pacmanMove(c, pacman, rows, cols, elems);    // move pacman
        pacmanPrint(pacman);    // print pacman
        drawMap(rows, cols, elems);    // draw the board
        
        c = getInput(start, updateRate);
    }
    

    endwin();

    return 0;
}