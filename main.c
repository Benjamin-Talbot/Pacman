#include <stdio.h>
#include <ncurses.h>
#include "pacman.h"
#include "map.h"

// don't allow multiple getch()'s per timeout

int main() {

    initscr(); // creates stdscr
    timeout(200);


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
    char backspace = 8;
    
    drawMap(rows, cols, elems);
    pacmanPrint(pacman);

    while(1) {
        c = getch();
        printw("%c ", backspace);    // delete typed character
        pacmanMove(c, pacman, rows, cols, elems);    // move pacman
        pacmanPrint(pacman);    // print pacman
        drawMap(rows, cols, elems);    // draw the board
    }
    

    endwin();

    return 0;
}