#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#include "pacman/pacmanFunctions.h"
#include "map/mapFunctions.h"
#include "powerup/powerupFunctions.h"
#include "ghost/ghostFunctions.h"
#include "mainFunctions/mainFunctions.h"
#include "score/score.h"

int main() {
    char* player = getName(player);
    fflush(stdout);

    srand(time(NULL));
    clock_t start;
    int updateRate = 100;   // set to 150 or 200
    int pauseTime = 1;
    char c;
    pPacman pacman = NULL;
    // int score = 0;
    pMap map = NULL;
    int maxLevel = 2;
    int level = 0; level++; //level++;
    pPowerup* powerups = NULL;
    int* numPowerups = NULL;
    pGhost ghosts = NULL;
    int* numGhosts = NULL;

    initscr();
    curs_set(0);
    nodelay(stdscr, true);
    noecho();
    
    initialize(&pacman, 0, &map, level, &powerups, &numPowerups, &ghosts, &numGhosts, pauseTime);

    while(!pacman->gameover) {
        start = clock();

        if(pacman->won)
            level = nextLevel(&pacman, &map, level, maxLevel, &powerups, &numPowerups, &ghosts, &numGhosts, pauseTime);

        if(!pacman->gameover) {
            update(pacman, c, map, powerups, numPowerups, ghosts, *numGhosts);
            draw(pacman, map, powerups, *numPowerups, ghosts, *numGhosts);
        }

        c = getInput(start, updateRate, pacman);
    }
    sleep(pauseTime);
    endwin();

    endGame(pacman->score, player);
    freeMemory(pacman, map, powerups, numPowerups, ghosts, numGhosts);
    free(player);

    return 0;
}