#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include "pacman/pacman.h"
#include "pacman/pacmanFunctions.h"
#include "map/map.h"
#include "map/mapFunctions.h"
#include "powerup/powerup.h"
#include "powerup/powerupFunctions.h"
#include "ghost/ghost.h"
#include "ghost/ghostFunctions.h"
#include "mainFunctions/mainFunctions.h"

// add fruit (cherries, strawberries, etc.)
// automate the process of creating powerups and ghosts?
// check misc.txt for order of operations

int main() {
    clock_t start;
    int updateRate = 150;
    // updateRate = 500;
    char c;
    const int rows = 15, cols = 40;
    char* elems = malloc(sizeof(char) * rows * cols);
    int numPowerups = 1;
    int numGhosts = 4;
    
    elems = loadMap(elems, rows, cols, 1);
    pMap map = (pMap) malloc(sizeof(Map));
    mapInit(map, rows, cols, elems);
    pPacman pacman = (pPacman) malloc(sizeof(Pacman));
    pacmanInit(pacman);
    pPowerup powerups = (pPowerup) malloc(sizeof(Powerup) * numPowerups);
    powerupInit(powerups, 2, 1);
    pGhost ghosts = (pGhost) malloc(sizeof(Ghost) * numGhosts);
    ghostInit(ghosts, 28, 3);
    ghostInit(ghosts+1, 27, 5);
    ghostInit(ghosts+2, 28, 5);
    ghostInit(ghosts+3, 29, 5);

    initscr();    // creates stdscr, start code dealing with ncurses


    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();

    while(1) {
        start = clock();

        update(pacman, c, map, powerups, numPowerups, ghosts, numGhosts);
        draw(pacman, map, powerups, numPowerups, ghosts, numGhosts);
        
        c = getInput(start, updateRate, pacman);
    }
    

    endwin();    // end code dealing with ncurses

    return 0;
}