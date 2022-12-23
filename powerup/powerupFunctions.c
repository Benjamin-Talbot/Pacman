#include "powerupFunctions.h"

// initializes the powerup struct
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

// frees a powerup and removes it from the array of powerups
void powerupDelete(pPowerup* powerups, int* numPowerups, int index) {
    free(powerups[index]);
    for(int i = index; i < *numPowerups - 1; i++) {
        powerups[i] = powerups[i+1];
    }
    (*numPowerups)--;
}

// draws a powerup
void powerupDraw(pPowerup powerup) {
    mvprintw(powerup->y, powerup->x, "%c", powerup->sprites[powerup->sprite]);
    powerup->sprite++;
    if(powerup->sprite >= powerup->numSprites)
        powerup->sprite = 0;
}

// draws all the powerups
void powerupsDraw(pPowerup* powerups, int numPowerups) {
    for(int i = 0; i < numPowerups; i++) {
        powerupDraw(powerups[i]);
    }
}