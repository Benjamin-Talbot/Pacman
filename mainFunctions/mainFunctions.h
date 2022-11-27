#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>

#include "../pacman/pacmanFunctions.h"
#include "../map/mapFunctions.h"
#include "../powerup/powerupFunctions.h"
#include "../ghost/ghostFunctions.h"

void replace_char(char str[], char oldc, char newc, int max);

char* loadMap(pMap* map, int level);

void initialize(pPacman* pacman, pMap* map, int level, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts);

char getInput(clock_t start, int updateRate, pPacman pacman);

void update(pPacman pacman, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts);

void draw(pPacman pacman, pMap map, pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts);

#endif