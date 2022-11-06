#include <ncurses.h>
#include "pacman.h"

int collides(Pacman* player, Map* map) {
    return 0;
}

void pacmanMove(char c, pPacman this, int rows, int cols, char elems[rows][cols]) {
    char sprite = this->sprite;
    int oldDirection[2] = {this->direction[0], this->direction[1]};
    int oldx = this->x, oldy = this->y;
    this->nextDirection[0] = 0;
    this->nextDirection[1] = 0;
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
    this->x += this->direction[0];
    this->y += this->direction[1];

    if(elems[this->y][this->x] != 35) {
        this->sprite = sprite;
    }
    else {
        this->nextDirection[0] = this->direction[0];
        this->nextDirection[1] = this->direction[1];
        this->x = oldx + oldDirection[0];
        this->y = oldy + oldDirection[1];
        if(elems[this->y][this->x] != 35) {
            this->direction[0] = oldDirection[0];
            this->direction[1] = oldDirection[1];
        }
        else {
            this->x = oldx;
            this->y = oldy;
            this->direction[0] = 0;
            this->direction[1] = 0;
        }
    }
}

void pacmanPrint(pPacman this) {
    mvprintw(this->y - this->direction[1], this->x - this->direction[0], " ");    // erase pacman from previous position
    mvprintw(this->y, this->x, "%c", this->sprite);    // draw pacman at new position
}

void pacmanInit(pPacman this) {
    this->x = 1;
    this->y = 1;
    this->direction[0] = 0;
    this->direction[1] = 0;
    this->nextDirection[0] = 0;
    this->nextDirection[1] = 0;
    this->sprite = 60;
}