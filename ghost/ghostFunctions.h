#ifndef GHOST_FUNCTIONS_H
#define GHOST_FUNCTIONS_H

#include "ghost.h"

#include <ncurses.h>
#include <stdlib.h>

#include "../pacman/pacmanFunctions.h"
#include "../map/mapFunctions.h"

#ifndef WALL
#define WALL '#'    // #, 35
#endif
#ifndef DOOR
#define DOOR '-'    // -, 45
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
#undef ALLDIR
#define ALLDIR(dir) this->dir[0] = 1; this->dir[1] = 1;

void ghostInit(pGhost this, int x, int y);

char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]);

void ghostMoveOptions(pGhost this, pMap map, char elems[map->height][map->width], char withBackwards);

void ghostWander(pGhost this);

void ghostFollowPacman(pGhost this, pPacman pacman, pMap map);

char ghostHitsPacman(pGhost this, pPacman pacman);

void ghostMove(pGhost this, pPacman pacman, pMap map);

void ghostsMove(pGhost ghosts, int numGhosts, pPacman pacman, pMap map);

void ghostsDraw(pGhost ghosts, int numGhosts, pMap map, char elems[map->height][map->width]);

#endif