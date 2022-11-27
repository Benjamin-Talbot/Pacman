#ifndef GHOST_FUNCTIONS_H
#define GHOST_FUNCTIONS_H

#include "ghost.h"

#include <ncurses.h>

#include "../pacman/pacmanFunctions.h"
#include "../map/mapFunctions.h"

void ghostInit(pGhost this, int x, int y);

// char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]);

// char ghostAtIntersection(pGhost this);

// void ghostMove(pGhost this, pPacman pacman, pMap map);

void ghostsDraw(pGhost ghosts, int numGhosts);

#endif