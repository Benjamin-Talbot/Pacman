#include <stdio.h>
#include <ncurses.h>
#include <time.h>

#include "pacman/pacmanFunctions.h"
#include "map/mapFunctions.h"
#include "powerup/powerupFunctions.h"
#include "ghost/ghostFunctions.h"
#include "mainFunctions/mainFunctions.h"

int main() {
    srand(time(NULL));
    clock_t start;
    int updateRate = 200;   // set to 150 or 200
    char c;
    pPacman pacman = NULL;
    pMap map = NULL;
    int level = 0;
    pPowerup* powerups = NULL;
    int* numPowerups = NULL;
    pGhost ghosts = NULL;
    int* numGhosts = NULL;

    initialize(&pacman, &map, level, &powerups, &numPowerups, &ghosts, &numGhosts);

    initscr();    // creates stdscr, start code dealing with ncurses

    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();

    draw(pacman, map, powerups, *numPowerups, ghosts, *numGhosts);
    while(1) {
        start = clock();

        update(pacman, c, map, powerups, numPowerups, ghosts, *numGhosts);
        draw(pacman, map, powerups, *numPowerups, ghosts, *numGhosts);
        
        c = getInput(start, updateRate, pacman);
    }

    endwin();    // end code dealing with ncurses

    return 0;
}

/*
variables for free():
- pacman
- ghosts
- powerups
- powerup[i]
- directions
*/