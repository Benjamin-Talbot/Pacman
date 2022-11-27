#include "powerupFunctions.h"

void powerupInit(pPowerup this, int x, int y) {
    this->x = x;
    this->y = y;
    this->sprites[0] = '-';
    this->sprites[1] = '\\';
    this->sprites[2] = '|';
    this->sprites[3] = '/';
    this->sprite = 0;
    this->numSprites = 4;
}

void powerupDelete(pPowerup* powerups, int* numPowerups, int index) {
    free(powerups[index]);
    for(int i = index; i < *numPowerups - 1; i++) {
        powerups[i] = powerups[i+1];
    }
    (*numPowerups)--;
}

void powerupsDraw(pPowerup* powerups, int numPowerups) {
    for(int i = 0; i < numPowerups; i++) {
        mvprintw(powerups[i]->y, powerups[i]->x, "%c", powerups[i]->sprites[powerups[i]->sprite]);
        powerups[i]->sprite++;
        if(powerups[i]->sprite >= powerups[i]->numSprites)
            powerups[i]->sprite = 0;
    }
}