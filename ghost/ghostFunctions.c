#include "ghostFunctions.h"

#include <time.h>

void ghostInit(pGhost this, int x, int y) {
    this->x = x;
    this->y = y;
    this->initx = x;
    this->inity = y;
    ALLDIR(direction);
    STOP(oldDirection);
    STOP(nextDirection);
    this->turnx = 0;
    this->turny = 0;
    this->stopTracking = FALSE;
    this->sprite = '&';
    this->cooldown = 0;    // used so ghosts can't eat pacman immediately after being reset
    this->trackingPacman = 0;
    this->directions = (int**) malloc(sizeof(int*) * 4);
    for(int i = 0; i < 4; i++) {
        this->directions[i] = malloc(sizeof(int) * 2);
    }
    this->numChoices = 0;
    this->vulnerable = FALSE;
    this->toggleUpdate = 1;
}

void ghostReset(pGhost this) {
    this->x = this->initx;
    this->y = this->inity;
    ALLDIR(direction);
    STOP(oldDirection);
    STOP(nextDirection);
    this->turnx = 0;
    this->turny = 0;
    this->stopTracking = FALSE;
    this->trackingPacman = 0;
    this->vulnerable = FALSE;
    this->toggleUpdate = 1;
    this->cooldown = 10;
}

char withinBounds(int x, int y, int width, int height) {
    if(x >= 0 && x < width && y >= 0 && y < height)
        return TRUE;
    return FALSE;
}

char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]) {
    int x = this->x + 1;
    int y = this->y;
    char foundPacman = FALSE;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPacman && withinBounds(x, y, map->width, map->height)) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = TRUE;
        }
        x++;
    }
    x = this->x;
    y = this->y - 1;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPacman && withinBounds(x, y, map->width, map->height)) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = TRUE;
        }
        y--;
    }
    x = this->x - 1;
    y = this->y;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPacman && withinBounds(x, y, map->width, map->height)) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = TRUE;
        }
        x--;
    }
    x = this->x;
    y = this->y + 1;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPacman && withinBounds(x, y, map->width, map->height)) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = TRUE;
        }
        y++;
    }

    if(!foundPacman)
        this->stopTracking = FALSE;
    else
        if(this->stopTracking)
            foundPacman = FALSE;


    return foundPacman;
}

void ghostMoveOptions(pGhost this, pMap map, char elems[map->height][map->width], char withBackwards) {
    this->numChoices = 0;

    if(this->direction[0]) {    // if going in x direction, check y for intersection
        if(elems[this->y-1][this->x] != WALL && elems[this->y-1][this->x] != DOOR) {
            this->directions[this->numChoices][0] = 0;
            this->directions[this->numChoices][1] = -1;
            this->numChoices++;
        }
        if(elems[this->y+1][this->x] != WALL && elems[this->y+1][this->x] != DOOR) {
            this->directions[this->numChoices][0] = 0;
            this->directions[this->numChoices][1] = 1;
            this->numChoices++;
        }
    }
    else if(this->direction[1]) {    // going in y direction, check x for intersection
        if(elems[this->y][this->x-1] != WALL && elems[this->y][this->x-1] != DOOR) {
            this->directions[this->numChoices][0] = -1;
            this->directions[this->numChoices][1] = 0;
            this->numChoices++;
        }
        if(elems[this->y][this->x+1] != WALL && elems[this->y][this->x+1] != DOOR) {
            this->directions[this->numChoices][0] = 1;
            this->directions[this->numChoices][1] = 0;
            this->numChoices++;
        }
    }

    int nextx = this->x + this->direction[0];
    int nexty = this->y + this->direction[1];
    if(elems[nexty][nextx] != WALL && elems[nexty][nextx] != DOOR) {
        this->directions[this->numChoices][0] = this->direction[0];
        this->directions[this->numChoices][1] = this->direction[1];
        this->numChoices++;
    }

    if(withBackwards) {
        this->directions[this->numChoices][0] = this->direction[0] * -1;
        this->directions[this->numChoices][1] = this->direction[1] * -1;
        this->numChoices++;
    }
}

void ghostWander(pGhost this) {
    if(this->numChoices > 0) {    // the ghost can at least go forward
        int r = rand() % this->numChoices;
        this->direction[0] = this->directions[r][0];
        this->direction[1] = this->directions[r][1];
    }
    else {    // this means the only way to go is backwards
        this->direction[0] *= -1;
        this->direction[1] *= -1;
    }
}

void ghostFollowPacman(pGhost this, pPacman pacman, pMap map) {
    int r = rand() % 15;
    if(r == 14) {    // random chance to not follow
        ghostMoveOptions(this, map, map->elems, TRUE);
        ghostWander(this);
    }
    else {
        int xdir = pacman->x - this->x;
        int ydir = pacman->y - this->y;
        xdir /= xdir > 0 ? xdir : xdir * -1;
        ydir /= ydir > 0 ? ydir : ydir * -1;

        if(xdir && ydir) {  // pacman is around a turn or something
            this->turnx = pacman->x - pacman->direction[0];
            this->turny = pacman->y - pacman->direction[1];
            this->nextDirection[0] = pacman->direction[0];
            this->nextDirection[1] = pacman->direction[1];
        }
        else {
            this->direction[0] = xdir;
            this->direction[1] = ydir;
        }
    }
}

char ghostHitsPacman(pGhost this, pPacman pacman) {
    if(this->x == pacman->x && this->y == pacman->y)
        return TRUE;
    return FALSE;
}

// to prevent ghosts from going through walls
char ghostHitsWall(pGhost this, pMap map, char elems[map->height][map->width]) {
    if(elems[this->y + this->direction[1]][this->x + this->direction[0]] == WALL) {
        return TRUE;
    }

    return FALSE;
}

void ghostRunAway(pGhost this, pMap map, char elems[map->height][map->width], pPacman pacman) {
        this->trackingPacman = 0;

        ghostMoveOptions(this, map, map->elems, FALSE);
        int xdist = 0;
        int ydist = 0;
        int dist = 0;
        
        xdist = this->x + this->directions[0][0] - pacman->x;
        ydist = this->y + this->directions[0][1] - pacman->y;
        int farthestDist = xdist * xdist + ydist * ydist;
        int farthest = 0;

        for(int i = 1; i < this->numChoices; i++) {
            xdist = this->x + this->directions[i][0] - pacman->x;
            ydist = this->y + this->directions[i][1] - pacman->y;
            dist = xdist * xdist + ydist * ydist;
            
            if(dist > farthestDist) {
                farthestDist = dist;
                farthest = i;
            }
        }

        // int r = rand() % 5;
        // if(r == 4) {
        //     (this->numChoices)--;
        //     farthest = rand() % this->numChoices;
        // }
        
        this->direction[0] = this->directions[farthest][0];
        this->direction[1] = this->directions[farthest][1];
}

// implement bounds checking?
void ghostMove(pGhost this, pPacman pacman, pMap map) {
    if(!this->vulnerable) {    // this->vulnerable == FALSE
        if(this->trackingPacman) {
            this->trackingPacman = ghostSeesPacman(this, pacman, map, map->elems);
            if(!this->trackingPacman) {    // lost track
                ghostFollowPacman(this, pacman, map);
                this->turnx = this->turny = 0;
            }
            else {    // else continue following with random chance to stop
                int r = rand() % 30;
                if(r == 29) {
                    ghostMoveOptions(this, map, map->elems, TRUE);
                    ghostWander(this);
                    this->trackingPacman = FALSE;
                    this->stopTracking = TRUE;
                }
            }
        }
        else {
            this->trackingPacman = ghostSeesPacman(this, pacman, map, map->elems);
            if(this->trackingPacman) {    // starts following
                ghostFollowPacman(this, pacman, map);
            }
            else {    // isn't following
                if(this->turnx && this->turny) {
                    if(this->x == this->turnx && this->y == this->turny) {
                        this->direction[0] = this->nextDirection[0];
                        this->direction[1] = this->nextDirection[1];
                        this->turnx = this->turny = 0;
                    }
                }
                else{
                    ghostMoveOptions(this, map, map->elems, FALSE);
                    ghostWander(this);
                }
            }
        }
    }
    else {    // this->vulnerable == TRUE
        ghostRunAway(this, map, map->elems, pacman);
    }

    // to prevent ghosts from going through walls
    if(ghostHitsWall(this, map, map->elems)) {
        ghostMoveOptions(this, map, map->elems, TRUE);
        ghostWander(this);
    }

    this->x += this->direction[0];
    this->y += this->direction[1];

    if(ghostHitsPacman(this, pacman) && !this->cooldown)
        if(!pacman->invincible)
            gameover(pacman);
        else
            pacmanEatGhost(pacman, this);
}

void ghostsMove(pGhost ghosts, int numGhosts, pPacman pacman, pMap map) {
    pGhost ghost;
    for(int i = 0; i < numGhosts; i++) {
        ghost = &ghosts[i];
        if(ghost->vulnerable) {
            ghost->toggleUpdate = 1 - ghost->toggleUpdate;
        }
        if(ghost->toggleUpdate)
            if(ghost->cooldown > 0)
                (ghost->cooldown)--;
            else
                ghostMove(ghost, pacman, map);
    }
}

void ghostsDraw(pGhost ghosts, int numGhosts, pMap map, char elems[map->height][map->width]) {
    Ghost ghost;
    for(int i = 0; i < numGhosts; i++) {
        ghost = ghosts[i];
        mvprintw(ghost.y, ghost.x, "%c", ghost.sprite);
    }
}