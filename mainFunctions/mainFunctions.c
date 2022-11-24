#include "mainFunctions.h"

void initialize(pPacman* pacman, 
                pMap* map, int rows, int cols, char* elems, 
                pPowerup* powerups, int numPowerups, int powerupCoords[numPowerups][2],
                pGhost* ghosts, int numGhosts, int ghostCoords[numGhosts][2]) {
    
    *pacman = (pPacman) malloc(sizeof(Pacman));
    pacmanInit(*pacman);

    *map = (pMap) malloc(sizeof(Map));
    mapInit(*map, rows, cols, elems);

    *powerups = (pPowerup) malloc(sizeof(Powerup) * numPowerups);
    for(int i = 0; i < numPowerups; i++) {
        powerupInit(*powerups+i, powerupCoords[i][0], powerupCoords[i][1]);
    }

    *ghosts = (pGhost) malloc(sizeof(Ghost) * numGhosts);
    for(int i = 0; i < numGhosts; i++) {
        ghostInit(*ghosts+i, ghostCoords[i][0], ghostCoords[0][1]);
    }
}

char getInput(clock_t start, int updateRate, pPacman pacman) {
    char nextc = getch();
    char c = ERR;

    while(nextc != ERR) {
        c = nextc;
        nextc = getch();
    }

    clock_t end = clock();
    long time_elapsed = (long) ((end - start)/(double)CLOCKS_PER_SEC*1000);

    while(time_elapsed < updateRate) {
        nextc = getch();
        if(nextc != ERR) {
            c = nextc;
        }
        end = clock();
        time_elapsed = (long) ((end - start)/(double)CLOCKS_PER_SEC*1000);
    }

    if(c == ERR)
        pacman->changedDirection = 0;
    else
        pacman->changedDirection = 1;
    
    return c;
}

void update(pPacman pacman, char c, pMap map, pPowerup powerups, int numPowerups, pGhost ghosts, int numGhosts) {
    char sprite = pacmanChangeDirection(pacman, c);    // change pacman's direction
    pacmanMove(pacman, sprite, map, powerups, numPowerups, ghosts, numGhosts);    // move pacman
}

void draw(pPacman pacman, pMap map, pPowerup powerups, int numPowerups, pGhost ghosts, int numGhosts) {
    drawMap(map, map->elems);    // draw the board
    for(int i = 0; i < numPowerups; i++)
        powerupDraw(powerups+i);
    ghostDraw(ghosts, numGhosts);
    pacmanDraw(pacman);    // print pacman
}