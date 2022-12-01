#ifndef GHOST_H
#define GHOST_H

typedef struct Ghost {
    int x;
    int y;
    int direction[2];
    int oldDirection[2];
    int nextDirection[2];
    int turnx;
    int turny;
    char stopTracking;
    char sprite;
    char nextSprite;
    int timer;
    char trackingPacman;
    int** directions;
    int numChoices;
} Ghost;
typedef Ghost* pGhost;

#endif