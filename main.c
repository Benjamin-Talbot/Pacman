#include <stdio.h>
#include <ncurses.h>
#include <time.h>

#include "pacman/pacmanFunctions.h"
#include "map/mapFunctions.h"
#include "powerup/powerupFunctions.h"
#include "ghost/ghostFunctions.h"
#include "mainFunctions/mainFunctions.h"
#include "score/score.h"

int main() {
    // char* player = getName(player);
    char* player = "Ben";

    srand(time(NULL));
    clock_t start;
    int updateRate = 100;   // set to 150 or 200
    char c;
    pPacman pacman = NULL;
    pMap map = NULL;
    int level = 0;
    pPowerup* powerups = NULL;
    int* numPowerups = NULL;
    pGhost ghosts = NULL;
    int* numGhosts = NULL;

    initialize(&pacman, &map, level, &powerups, &numPowerups, &ghosts, &numGhosts);

    /**
    initscr();    // creates stdscr, start code dealing with ncurses

    curs_set(0);
    nodelay(stdscr, true);
    noecho();

    drawWalls(map, map->elems);
    draw(pacman, map, powerups, *numPowerups, ghosts, *numGhosts);
    // while(!pacman->gameover) {
    while(pacman->gameover) {
        start = clock();

        update(pacman, c, map, powerups, numPowerups, ghosts, *numGhosts);
        draw(pacman, map, powerups, *numPowerups, ghosts, *numGhosts);
        
        c = getInput(start, updateRate, pacman);
    }

    endwin();    // end code dealing with ncurses
    //*/

    endGame(pacman->score, player);

    // clean up allocated memory //

    // global variable from ghostFunctions.c
    // extern int** directions;
    // for(int i = 0; i < 3; i++)
    //     free(directions[i]);
    // free(directions);

    // free(numGhosts);
    // free(ghosts);

    // for(int i = 0; i < *numPowerups; i++)
    //     free(powerups[i]);
    // free(numPowerups);
    // free(powerups);

    // free(map->elems);
    // free(map);

    // free(pacman);

    return 0;
}

/*
Left to deallocate:
- player
- nodes in tree
- tree
*/