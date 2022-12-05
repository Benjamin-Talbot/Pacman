#ifndef PACMAN_FUNCTIONS_H
#define PACMAN_FUNCTIONS_H

#include "pacman.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "../map/mapFunctions.h"
#include "../powerup/powerupFunctions.h"
#include "../ghost/ghostFunctions.h"

#ifndef WALL
#define WALL ('#')    // #, 35
#endif
#ifndef DOOR
#define DOOR ('-')    // -, 45
#endif
#ifndef PELLET
#define PELLET ('.')    // ., 46
#endif

#undef GO_LEFT
#define GO_LEFT(dir) this->dir[0] = -1; this->dir[1] = 0;
#undef GO_RIGHT
#define GO_RIGHT(dir) this->dir[0] = 1; this->dir[1] = 0;
#undef GO_UP
#define GO_UP(dir) this->dir[0] = 0; this->dir[1] = -1;
#undef GO_DOWN
#define GO_DOWN(dir) this->dir[0] = 0; this->dir[1] = 1;
#undef STOP
#define STOP(dir) this->dir[0] = 0; this->dir[1] = 0;

void pacmanInit(pPacman this, int x, int y, int score);

void gameover(pPacman this);

char pacmanChangeDirection(pPacman this, char c);

int pacmanCollides(pPacman player, pMap map, char elems[map->height][map->width]);

void pacmanMakeInvincible(pPacman this, pGhost ghosts, int numGhosts);

void increaseScore(pPacman this, int points);

int pacmanEat(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts);

void pacmanEatGhost(pPacman this, pGhost ghost);

char pacmanHitsGhost(pPacman this, pGhost ghosts, int numGhosts);

void pacmanMove(pPacman this, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts);

void pacmanDraw(pPacman this);

#endif