#include "pacman.h"

#define WALL '#'    // #, 35
#define DOOR '-'    // -, 45
#define FOOD '.'    // ., 46

void pacmanInit(pPacman this) {
    this->x = 1;
    this->y = 1;
    this->direction[0] = 0;
    this->direction[1] = 0;
    this->oldDirection[0] = 0;
    this->oldDirection[1] = 0;
    this->nextDirection[0] = 0;
    this->nextDirection[1] = 0;
    this->changedDirection = 0;
    this->sprite = 60;
    this->nextSprite = 60;
    this->score = 0;
}

char pacmanChangeDirection(pPacman this, char c) {
    char sprite = this->sprite;
    this->oldDirection[0] = this->direction[0];
    this->oldDirection[1] = this->direction[1];
    switch(c) {
            case 119:   // w
                this->direction[0] = 0;
                this->direction[1] = -1;
                sprite = 118;   // v
                break;
            case 97:    // a
                this->direction[0] = -1;
                this->direction[1] = 0;
                sprite = 62;    // >
                break;
            case 115:   // s
                this->direction[0] = 0;
                this->direction[1] = 1;
                sprite = 94;    // ^
                break;
            case 100:   // d
                this->direction[0] = 1;
                this->direction[1] = 0;
                sprite = 60;    // <
                break;
    }
    
    return sprite;
}

int pacmanCollides(pPacman this, pMap map, char elems[map->height][map->width]) {
    return elems[this->y][this->x] == WALL || elems[this->y][this->x] == DOOR ? 1 : 0;
}

int pacmanEat(pPacman this, pMap map, char elems[map->height][map->width]) {
    char ate = 0;
    if(elems[this->y][this->x] == FOOD) {
        this->score++;
        elems[this->y][this->x] = ' ';    // should this be in map?
        ate = 1;
    }
    return ate;
}

void pacmanMove(pPacman this, char sprite, pMap map) {
    int oldx = this->x, oldy = this->y;

    if(this->changedDirection || (!this->nextDirection[0] && !this->nextDirection[1])) {
        if(this->changedDirection) {
            this->nextDirection[0] = 0;
            this->nextDirection[1] = 0;
        }

        this->x += this->direction[0];
        this->y += this->direction[1];

        if(!pacmanCollides(this, map, map->elems)) {
            this->sprite = sprite;
        }
        else {
            this->nextDirection[0] = this->direction[0];
            this->nextDirection[1] = this->direction[1];
            this->nextSprite = sprite;
            this->x = oldx + this->oldDirection[0];
            this->y = oldy + this->oldDirection[1];
            if(!pacmanCollides(this, map, map->elems)) {
                this->direction[0] = this->oldDirection[0];
                this->direction[1] = this->oldDirection[1];
            }
            else {
                this->x = oldx;
                this->y = oldy;
                this->direction[0] = 0;
                this->direction[1] = 0;
            }
        }
    }
    else if(this->nextDirection[0] || this->nextDirection[1]) {
        this->x += this->nextDirection[0];
        this->y += this->nextDirection[1];

        if(!pacmanCollides(this, map, map->elems)) {
            this->sprite = this->nextSprite;
            this->direction[0] = this->nextDirection[0];
            this->direction[1] = this->nextDirection[1];
            this->nextDirection[0] = 0;
            this->nextDirection[1] = 0;
            this->changedDirection = 1;
        }
        else {
            this->x = oldx + this->oldDirection[0];
            this->y = oldy + this->oldDirection[1];

            if(pacmanCollides(this, map, map->elems)) {
                this->x = oldx;
                this->y = oldy;
                this->direction[0] = this->nextDirection[0] = 0;
                this->direction[1] = this->nextDirection[1] = 0;
            }
        }
    }

    pacmanEat(this, map, map->elems);
}

void pacmanDraw(pPacman this) {
    mvprintw(this->y - this->direction[1], this->x - this->direction[0], " ");    // erase pacman from previous position
    mvprintw(this->y, this->x, "%c", this->sprite);    // draw pacman at new position
}