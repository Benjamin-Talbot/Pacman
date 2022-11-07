#include "ghost.h"

void ghostInit(pGhost this, int x, int y) {
    this->x = x;
    this->y = y;
    this->direction[0] = 0;
    this->direction[1] = 0;
    this->oldDirection[0] = 0;
    this->oldDirection[1] = 0;
    this->sprite = '&';
    this->timer = 20;
}

void ghostDraw(pGhost ghosts, int numGhosts) {
    for(int i = 0; i < numGhosts; i++)
        mvprintw((ghosts+i)->y, (ghosts+i)->x, "%c", (ghosts+i)->sprite);
}