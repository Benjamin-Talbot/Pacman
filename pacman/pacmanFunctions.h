#ifndef PACMAN_FUNCTIONS_H
#define PACMAN_FUNCTIONS_H

#include "pacman.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#include "../map/mapFunctions.h"
#include "../powerup/powerupFunctions.h"
#include "../ghost/ghostFunctions.h"

void pacmanInit(pPacman this);

void gameover();

char pacmanChangeDirection(pPacman this, char c);

int pacmanCollides(pPacman player, pMap map, char elems[map->height][map->width]);

void pacmanMakeInvincible(pPacman this);

int pacmanEat(pPacman this, pMap map, char elems[map->height][map->width], pPowerup powerups, int numPowerups);

void pacmanMove(pPacman this, char c, pMap map, pPowerup powerups, int numPowerups, pGhost ghosts, int numGhosts);

void pacmanDraw(pPacman this);

#endif