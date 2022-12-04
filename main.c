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

    srand(time(NULL));
    clock_t start;
    int updateRate = 100;   // set to 150 or 200
    char c;
    pPacman pacman = NULL;
    int score = 0;
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
    
    initialize(&pacman, score, &map, level, &powerups, &numPowerups, &ghosts, &numGhosts);

    while(!pacman->gameover) {
        start = clock();

        if(pacman->won) {
            sleep(1);
            clearMap(map, map->elems);
            level++;
            if(level <= maxLevel) {
                score = pacman->score;
                freeMemory(pacman, map, powerups, numPowerups, ghosts, numGhosts);
                initialize(&pacman, score, &map, level, &powerups, &numPowerups, &ghosts, &numGhosts);
            }
            else
                pacman->gameover = TRUE;
        }

        if(!pacman->gameover) {
            update(pacman, c, map, powerups, numPowerups, ghosts, *numGhosts);
            draw(pacman, map, powerups, *numPowerups, ghosts, *numGhosts);
        }

        c = getInput(start, updateRate, pacman);
    }
    sleep(1);
    endwin();

    endGame(pacman->score, player);
    freeMemory(pacman, map, powerups, numPowerups, ghosts, numGhosts);

    return 0;
}