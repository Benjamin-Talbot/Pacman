#ifndef GHOST_H
#define GHOST_H

#include <ncurses.h>

typedef struct Ghost {
    int x;
    int y;
    int direction[2];
    int oldDirection[2];
    char sprite;
    char nextSprite;
    int timer;
} Ghost;
typedef Ghost* pGhost;

void ghostInit(pGhost this, int x, int y);

void ghostDraw(pGhost this, int numGhosts);

#endif

/*
&
m n
*/