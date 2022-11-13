#ifndef POWERUP_H
#define POWERUP_H

typedef struct Powerup {
    int x;
    int y;
    char sprites[4];
    int sprite;
    int numSprites;
} Powerup;
typedef Powerup* pPowerup;

#endif