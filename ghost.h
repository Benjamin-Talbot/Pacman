#ifndef GHOST_H
#define GHOST_H

#include <ncurses.h>
#include "pacman.h"
#include "map.h"

typedef struct Ghost {
    int x;
    int y;
    int direction[2];
    int oldDirection[2];
    char sprite;
    char nextSprite;
    int timer;
    char trackingPacman;
} Ghost;
typedef Ghost* pGhost;

void ghostInit(pGhost this, int x, int y);

// char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]);

// char ghostAtIntersection(pGhost this);

// void ghostMove(pGhost this, pPacman pacman, pMap map);

void ghostDraw(pGhost this, int numGhosts);

#endif

/*
&
m n
*/