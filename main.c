#include <stdio.h>
#include <ncurses.h>
#include <time.h>

#include "pacman/pacmanFunctions.h"
#include "map/mapFunctions.h"
#include "powerup/powerupFunctions.h"
#include "ghost/ghostFunctions.h"
#include "mainFunctions/mainFunctions.h"

int main() {
    clock_t start;
    printf("hi");
    int updateRate = 150;
    char c;
    // const int rows = 15, cols = 40;
    const int rows = 23, cols = 47;
    char* elems = malloc(sizeof(char) * rows * cols);
    elems = loadMap(elems, rows, cols, 0);
    pPacman pacman = NULL;
    pMap map = NULL;
    pPowerup powerups = NULL;
    pGhost ghosts = NULL;

    int numPowerups = 1;
    int powerupCoords[numPowerups][2];
    powerupCoords[0][0] = 2;
    powerupCoords[0][1] = 1;
    
    int numGhosts = 4;
    int ghostCoords[numGhosts][2];
    ghostCoords[0][0] = 28;
    ghostCoords[0][1] = 3;
    ghostCoords[0][0] = 27;
    ghostCoords[0][1] = 5;
    ghostCoords[0][0] = 28;
    ghostCoords[0][1] = 5;
    ghostCoords[0][0] = 29;
    ghostCoords[0][1] = 5;
    
    // pMap map = (pMap) malloc(sizeof(Map));
    // mapInit(map, rows, cols, elems);
    // pPacman pacman = (pPacman) malloc(sizeof(Pacman));
    // pacmanInit(pacman);
    // pPowerup powerups = (pPowerup) malloc(sizeof(Powerup) * numPowerups);
    // powerupInit(powerups, 2, 1);
    // pGhost ghosts = (pGhost) malloc(sizeof(Ghost) * numGhosts);
    // ghostInit(ghosts, 28, 3);
    // ghostInit(ghosts+1, 27, 5);
    // ghostInit(ghosts+2, 28, 5);
    // ghostInit(ghosts+3, 29, 5);

    initialize(&pacman, &map, rows, cols, elems, &powerups, numPowerups, powerupCoords, &ghosts, numGhosts, ghostCoords);

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