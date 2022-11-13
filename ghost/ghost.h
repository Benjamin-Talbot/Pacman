#ifndef GHOST_H
#define GHOST_H

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

#endif

/*
&
m n
*/