#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "../pacman/pacmanFunctions.h"
#include "../map/mapFunctions.h"
#include "../powerup/powerupFunctions.h"
#include "../ghost/ghostFunctions.h"

void initialize(pPacman* pacman, 
                pMap* map, int rows, int cols, char* elems, 
                pPowerup* powerups, int numPowerups, int powerupCoords[numPowerups][2],
                pGhost* ghosts, int numGhosts, int ghostCoords[numGhosts][2]);

char getInput(clock_t start, int updateRate, pPacman pacman);

void update(pPacman pacman, char c, pMap map, pPowerup powerups, int numPowerups, pGhost ghosts, int numGhosts);

void draw(pPacman pacman, pMap map, pPowerup powerups, int numPowerups, pGhost ghosts, int numGhosts);

#endif