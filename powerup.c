#include "powerup.h"

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

void copyPowerup(pPowerup powerups[], int dest, int src) {
    *powerups[dest] = *powerups[src];
}

void powerupDelete(pPowerup powerups, int numPowerups, int index) {
    free(powerups+index);
    for(int i = 0; i < numPowerups; i++) {
        copyPowerup(&powerups, i, i+1);
    }
}

void powerupDraw(pPowerup this) {
    mvprintw(this->y, this->x, "%c", this->sprites[this->sprite]);
    this->sprite++;
    if(this->sprite >= this->numSprites)
        this->sprite = 0;
}