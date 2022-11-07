#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include "pacman.h"
#include "map.h"
#include "functions.h"
#include "powerup.h"

int main() {
    clock_t start;
    int updateRate = 200;
    char c;
    const int rows = 15, cols = 40;
    char* elems = malloc(sizeof(char) * rows * cols);
    int numPowerups = 0;
    
    elems = loadMap(elems, rows, cols, 1);
    pMap map = (pMap) malloc(sizeof(Map));
    mapInit(map, rows, cols, elems);
    pPacman pacman = (pPacman) malloc(sizeof(Pacman));
    pacmanInit(pacman);
    pPowerup powerups = (pPowerup) malloc(sizeof(Powerup));
    powerupInit(&powerups[0], 2, 1);

    initscr();    // creates stdscr, start code dealing with


    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();

    while(1) {
        start = clock();

        update(pacman, c, map);
        draw(pacman, map, powerups);
        
        c = getInput(start, updateRate, pacman);
    }
    

    endwin();    // end code dealing with n

    return 0;
}