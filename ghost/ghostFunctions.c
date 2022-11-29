#include "ghostFunctions.h"

#include <time.h>

#ifndef WALL
#define WALL '#'    // #, 35
#endif
#ifndef DOOR
#define DOOR '-'    // -, 45
#endif

void ghostInit(pGhost this, int x, int y) {
    this->x = x;
    this->y = y;
    this->direction[0] = 0;
    this->direction[1] = 1;
    this->oldDirection[0] = 0;
    this->oldDirection[1] = 0;
    this->sprite = '&';
    this->timer = 20;
    this->trackingPacman = 0;
}

char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]) {
    int x = this->x + 1, y = this->y;
    char foundPacman = 0;
    while(elems[y][x] != WALL && !foundPacman) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = 1;
        }
        x++;
    }
    x = this->x, y = this->y - 1;
    while(elems[y][x] != WALL && !foundPacman) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = 1;
        }
        y--;
    }
    x = this->x - 1, y = this->y;
    while(elems[y][x] != WALL && !foundPacman) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = 1;
        }
        x--;
    }
    x = this->x, y = this->y + 1;
    while(elems[y][x] != WALL && !foundPacman) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = 1;
        }
        y++;
    }

    return foundPacman;
}

void ghostMoveOptions(pGhost this, pMap map, char elems[map->height][map->width]) {
    numChoices = 0;
    if(!directions) {
        directions = (int**) malloc(sizeof(int*) * 3);
        for(int i = 0; i < 3; i++) {
            directions[i] = malloc(sizeof(int) * 2);
        }
    }

    if(this->direction[0]) {    // if going in x direction, check y for intersection
        if(elems[this->y-1][this->x] != WALL && elems[this->y-1][this->x] != DOOR) {
            directions[numChoices][0] = 0;
            directions[numChoices][1] = -1;
            numChoices++;
        }
        if(elems[this->y+1][this->x] != WALL && elems[this->y+1][this->x] != DOOR) {
            directions[numChoices][0] = 0;
            directions[numChoices][1] = 1;
            numChoices++;
        }
    }
    else if(this->direction[1]) {    // going in y direction, check x for intersection
        if(elems[this->y][this->x-1] != WALL && elems[this->y][this->x-1] != DOOR) {
            directions[numChoices][0] = -1;
            directions[numChoices][1] = 0;
            numChoices++;
        }
        if(elems[this->y][this->x+1] != WALL && elems[this->y][this->x+1] != DOOR) {
            directions[numChoices][0] = 1;
            directions[numChoices][1] = 0;
            numChoices++;
        }
    }

    int nextx = this->x + this->direction[0];
    int nexty = this->y + this->direction[1];
    if(elems[nexty][nextx] != WALL && elems[nexty][nextx] != DOOR) {
        directions[numChoices][0] = this->direction[0];
        directions[numChoices][1] = this->direction[1];
        numChoices++;
    }
}

void ghostWander(pGhost this) {
    if(numChoices) {    // the ghost can at least go forward
        int r = rand() % numChoices;
        this->direction[0] = directions[r][0];
        this->direction[1] = directions[r][1];
    }
    else {  // this means the only way to go is backwards
        this->direction[0] *= -1;
        this->direction[1] *= -1;
    }
}

void ghostFollowPacman() {

}

char ghostHitsPacman(pGhost this, pPacman pacman) {
    if(this->x == pacman->x && this->y == pacman->y)
        return TRUE;
    return FALSE;
}

void ghostMove(pGhost this, pPacman pacman, pMap map) {
    // move back down or keep here?
    // this->x += this->direction[0];
    // this->y += this->direction[1];

    if(this->trackingPacman) {
        // this->trackingPacman = ghostSeesPacman(this, pacman, map, map->elems);
        // ghostFollowPacman();
    }
    else {
        // this->trackingPacman = ghostSeesPacman(this, pacman, map, map->elems);
        if(this->trackingPacman) {
            ghostFollowPacman();
        }
        else {
            ghostMoveOptions(this, map, map->elems);
            ghostWander(this);
        }
    }

    this->x += this->direction[0];
    this->y += this->direction[1];

    if(ghostHitsPacman(this, pacman))
        gameover(pacman);     // change to a variable, maybe in pacman, in case of circular references?
}

void ghostsMove(pGhost ghosts, int numGhosts, pPacman pacman, pMap map) {
    for(int i = 0; i < numGhosts; i++) {
        ghostMove(&ghosts[i], pacman, map);
    }
}

void ghostsDraw(pGhost ghosts, int numGhosts, pMap map, char elems[map->height][map->width]) {
    Ghost ghost;
    for(int i = 0; i < numGhosts; i++) {
        ghost = ghosts[i];
        int oldx = ghost.x - ghost.direction[0];
        int oldy = ghost.y - ghost.direction[1];
        mvprintw(oldy, oldx, "%c", elems[oldy][oldx]);
    }
    for(int i = 0; i < numGhosts; i++) {
        ghost = ghosts[i];
        mvprintw(ghost.y, ghost.x, "%c", ghost.sprite);
    }
}