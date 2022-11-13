#ifndef POWERUP_FUNCTIONS_H
#define POWERUP_FUNCTIONS_H

#include "powerup.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void powerupInit(pPowerup this, int x, int y);

void copyPowerup(pPowerup powerups[], int dest, int src);

void powerupDelete(pPowerup this, int numPowerups, int i);

void powerupDraw(pPowerup this);

#endif