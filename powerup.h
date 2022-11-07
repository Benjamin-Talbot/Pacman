#ifndef POWERUP_H
#define POWERUP_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

typedef struct Powerup {
    int x;
    int y;
    char sprites[4];
    int sprite;
    int numSprites;
} Powerup;
typedef Powerup* pPowerup;

void powerupInit(pPowerup this, int x, int y);

void powerupDraw(pPowerup this);

#endif