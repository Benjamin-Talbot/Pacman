#ifndef POWERUP_FUNCTIONS_H
#define POWERUP_FUNCTIONS_H

#include "powerup.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void powerupInit(pPowerup this, int x, int y);

void powerupDelete(pPowerup* powerups, int* numPowerups, int index);

void powerupsDraw(pPowerup* powerups, int numPowerups);

#endif