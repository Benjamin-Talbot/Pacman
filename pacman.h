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

int collides(pPacman player, Map* map);

void pacmanMove(char c, pPacman this, int rows, int cols, char elems[rows][cols]);

void pacmanPrint(pPacman this);

void pacmanInit(pPacman this);

#endif