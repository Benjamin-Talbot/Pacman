#ifndef GHOST_FUNCTIONS_H
#define GHOST_FUNCTIONS_H

#include "ghost.h"

#include <ncurses.h>

#include "../pacman/pacmanFunctions.h"
#include "../map/mapFunctions.h"

// static int** directions = NULL;
// static int numChoices = 0;

void ghostInit(pGhost this, int x, int y);

char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]);

void ghostMoveOptions(pGhost this, pMap map, char elems[map->height][map->width]);

void ghostWander(pGhost this);

void ghostFollowPacman();

char ghostHitsPacman(pGhost this, pPacman pacman);

void ghostMove(pGhost this, pPacman pacman, pMap map);

void ghostsMove(pGhost ghosts, int numGhosts, pPacman pacman, pMap map);

void ghostsDraw(pGhost ghosts, int numGhosts, pMap map, char elems[map->height][map->width]);

#endif