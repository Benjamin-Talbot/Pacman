#include "ghostFunctions.h"

#ifndef WALL
#define WALL '#'    // #, 35
#endif

void ghostInit(pGhost this, int x, int y) {
    this->x = x;
    this->y = y;
    this->direction[0] = 0;
    this->direction[1] = 0;
    this->oldDirection[0] = 0;
    this->oldDirection[1] = 0;
    this->sprite = '&';
    this->timer = 20;
    this->trackingPacman = 0;
}

// char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]) {
//     int x = this->x + 1, y = this->y;
//     char foundPacman = 0;
//     while(elems[y][x] != WALL && !foundPacman) {
//         if(x == pacman->x && y == pacman->y) {
//             foundPacman = 1;
//         }
//         x++;
//     }
//     x = this->x, y = this->y - 1;
//     while(elems[y][x] != WALL && !foundPacman) {
//         if(x == pacman->x && y == pacman->y) {
//             foundPacman = 1;
//         }
//         y--;
//     }
//     x = this->x - 1, y = this->y;
//     while(elems[y][x] != WALL && !foundPacman) {
//         if(x == pacman->x && y == pacman->y) {
//             foundPacman = 1;
//         }
//         x--;
//     }
//     x = this->x, y = this->y + 1;
//     while(elems[y][x] != WALL && !foundPacman) {
//         if(x == pacman->x && y == pacman->y) {
//             foundPacman = 1;
//         }
//         y++;
//     }

//     return foundPacman;
// }

// char ghostAtIntersection(pGhost this) {
//     char atIntersection = 0;
//     if(this->direction[0]) {    // if going in x direction, check y for intersection
//         if(this->y - 1 != WALL || this->y + 1 != WALL) {
//             atIntersection = 1;
//         }
//     }
//     else {    // going in y direction, check x for intersection
//         if(this->x - 1 != WALL || this->x + 1 != WALL) {
//             atIntersection = 1;
//         }
//     }

//     return atIntersection;
// }

// void ghostMove(pGhost this, pPacman pacman, pMap map) {
//     if(this->trackingPacman) {
//         this->trackingPacman = ghostSeesPacman(this, pacman, map, map->elems);
//     }
//     else {
//         this->trackingPacman = ghostSeesPacman(this, pacman, map, map->elems);
//     }
// }

void ghostsDraw(pGhost ghosts, int numGhosts) {
    for(int i = 0; i < numGhosts; i++)
        mvprintw(ghosts[i].y, ghosts[i].x, "%c", ghosts[i].sprite);
}