#ifndef GHOST_H
#define GHOST_H

typedef struct Ghost {
    int x;
    int y;
    int initx;
    int inity;
    int direction[2];
    int oldDirection[2];
    int nextDirection[2];
    int turnx;
    int turny;
    char stopTracking;
    char sprite;
    char nextSprite;
    int cooldown;
    char trackingPacman;
    int** directions;
    int numChoices;
    char vulnerable;
    int toggleUpdate;
} Ghost;
typedef Ghost* pGhost;

#endif