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
#include "portal/portalFunctions.h"

int main() {
    char CPU;
    CPU = getPlayer();
    char* player = "CPU";
    if(!CPU)
        player = getName(player);
    fflush(stdout);

    srand(time(NULL));
    clock_t start;
    int updateRate = 150;
    int pauseTime = 1;
    char c = '\0';
    pPacman pacman = NULL;
    pMap map = NULL;
    int maxLevel = 2;
    int level = 0;
    pPowerup* powerups = NULL;
    int* numPowerups = NULL;
    pGhost ghosts = NULL;
    int* numGhosts = NULL;
    pPortal* portals = NULL;
    int numPortals = 0;

    initscr();
    curs_set(0);
    nodelay(stdscr, true);
    noecho();
    
    initialize(&pacman, CPU, 0, &map, level, &powerups, &numPowerups, &ghosts, &numGhosts, &portals, &numPortals, pauseTime);

    while(!pacman->gameover) {
        start = clock();

        if(pacman->won)
            level = nextLevel(&pacman, &map, level, maxLevel, &powerups, &numPowerups, &ghosts, &numGhosts, &portals, &numPortals, pauseTime);

        clear();
        update(pacman, c, map, powerups, numPowerups, ghosts, *numGhosts, portals, numPortals);
        draw(pacman, map, powerups, *numPowerups, ghosts, *numGhosts, portals, numPortals);

        c = getInput(start, updateRate, pacman);
    }
    sleep(pauseTime);
    endwin();

    endGame(pacman->score, player);
    freeMemory(pacman, map, powerups, numPowerups, ghosts, numGhosts);
    if(!CPU)
        free(player);

    return 0;
}

/*
cd C/FinalProject; make clean; make main; clear; ./main
cd C/FinalProject; make clean; make main; clear; lldb main
*/