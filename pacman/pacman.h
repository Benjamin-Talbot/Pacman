#ifndef PACMAN_H
#define PACMAN_H

typedef struct Pacman {
    int x;
    int y;
    int direction[2];
    int oldDirection[2];
    int nextDirection[2];
    char changedDirection;
    char sprite;
    char nextSprite;
    int score;
    int invincible;
} Pacman;
typedef Pacman* pPacman;

#endif