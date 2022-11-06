#ifndef PACMAN_H
#define PACMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "map.h"

typedef struct {
    int x;
    int y;
    int direction[2];
    int nextDirection[2];
    char sprite;
    // void (*move)(struct Pacman* player, int dx, int dy, map* map);
} Pacman;
typedef Pacman* pPacman;

void pacmanInit(pPacman this);

int collides(pPacman player, Map* map);

void pacmanMove(pPacman this, char c, pMap map, char elems[map->height][map->width]);

void pacmanDraw(pPacman this);

#endif