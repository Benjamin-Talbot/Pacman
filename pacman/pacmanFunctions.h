#ifndef PACMAN_FUNCTIONS_H
#define PACMAN_FUNCTIONS_H

#include "pacman.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>

#include "../map/mapFunctions.h"
#include "../powerup/powerupFunctions.h"
#include "../ghost/ghostFunctions.h"
#include "../portal/portalFunctions.h"

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

// begin pacman AI functions
int randint(int max);
void pacmanMoveOptions(pPacman this, pMap map, char elems[map->height][map->width], char withBackwards);
int pacmanSeesPellet(pPacman this, pMap map, char elems[map->height][map->width], int dirs[4][2], int* choice, pPowerup* powerups, int numPowerups);
void pelletVector(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, float* pelletxcomp, float* pelletycomp);
void findNearestPellet(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, int* minx, int* miny);
char ghostsInVicinity(pPacman this, pGhost ghosts, int numGhosts, int* minx, int* miny, int* ghostxcomp, int* ghostycomp);
char pacmanChooseDirection(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts, char *c);
// end pacman AI functions

void pacmanInit(pPacman this, int x, int y, int score, char CPU);
void gameover(pPacman this);
char pacmanChangeDirection(pPacman this, char c);
int pacmanCollides(pPacman player, pMap map, char elems[map->height][map->width]);
void pacmanMakeInvincible(pPacman this, pGhost ghosts, int numGhosts);
void increaseScore(pPacman this, int points);
int pacmanEat(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts);
void pacmanAtPortal(pPacman this, pPortal* portals, int numPortals);
void pacmanEatGhost(pPacman this, pGhost ghost);
char pacmanHitsGhost(pPacman this, pGhost ghosts, int numGhosts);
void pacmanUninvincible(pGhost ghosts, int numGhosts);
void pacmanMove(pPacman this, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts, pPortal* portals, int numPortals);
void pacmanDraw(pPacman this);

#endif