#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <time.h>
#include "pacman.h"
#include "powerup.h"
#include "ghost.h"

char getInput(clock_t start, int updateRate, pPacman pacman);

void update(pPacman pacman, char c, pMap map, pPowerup powerups, int numPowerups, pGhost ghosts, int numGhosts);

void draw(pPacman pacman, pMap map, pPowerup powerups, int numPowerups, pGhost ghosts, int numGhosts);

#endif