#ifndef PACMAN_H
#define PACMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "map.h"

typedef struct Pacman {
    int x;
    int y;
    int direction[2];
    int oldDirection[2];
    int nextDirection[2];
    char changedDirection;
    char sprite;
    char nextSprite;
    // void (*move)(struct Pacman* player, int dx, int dy, map* map);
} Pacman;
typedef Pacman* pPacman;

void pacmanInit(pPacman this);

char pacmanChangeDirection(pPacman this, char c);

int pacmanCollides(pPacman player, pMap map, char elems[map->height][map->width]);

void pacmanMove(pPacman this, char c, pMap map);

void pacmanDraw(pPacman this);

#endif