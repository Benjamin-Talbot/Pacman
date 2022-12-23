#include "portalFunctions.h"

// initialize the portal struct
void portalInit(pPortal this, int x, int y) {
    this->x = x;
    this->y = y;
    this->sprites[0] = 'O';
    this->sprites[1] = 'o';
    this->sprite = 0;
    this->exit = NULL;
}

// link two portals together
void portalLink(pPortal first, pPortal second) {
    first->exit = second;
    second->exit = first;
    second->sprite = 1 - second->sprite;
}

// teleport Pacman from one portal to another
void teleport(pPortal this, pPacman pacman) {
    if(this->exit) {
        pacman->x = this->exit->x;
        pacman->y = this->exit->y;
    }
}

// draws the portal
void portalDraw(pPortal this) {
    mvprintw(this->y, this->x, "%c", this->sprites[this->sprite]);
    this->sprite = 1 - this->sprite;
}

// draws all portals
void portalsDraw(pPortal* portals, int numPortals) {
    for(int i = 0; i < numPortals; i++)
        portalDraw(portals[i]);
}