#include "ghostFunctions.h"

#include <time.h>

static int debug = 0;

void ghostInit(pGhost this, int x, int y) {
    this->x = x;
    this->y = y;
    ALLDIR(direction);
    STOP(oldDirection);
    STOP(nextDirection);
    int turnx = 0;
    int turny = 0;
    this->stopTracking = FALSE;
    this->sprite = '&';
    this->timer = 20;
    this->trackingPacman = 0;
    this->directions = (int**) malloc(sizeof(int*) * 4);
    for(int i = 0; i < 4; i++) {
        this->directions[i] = malloc(sizeof(int) * 2);
    }
    this->numChoices = 0;
}

char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]) {
    int x = this->x + 1, y = this->y;
    char foundPacman = FALSE;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPacman) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = TRUE;
        }
        x++;
    }
    x = this->x, y = this->y - 1;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPacman) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = TRUE;
        }
        y--;
    }
    x = this->x - 1, y = this->y;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPacman) {
        if(x == pacman->x && y == pacman->y) {
            foundPacman = TRUE;
        }
        x--;
    }
    x = this->x, y = this->y + 1;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPacman) {
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
    if(this->numChoices) {    // the ghost can at least go forward
        int r = rand() % this->numChoices;
        this->direction[0] = this->directions[r][0];
        this->direction[1] = this->directions[r][1];
    }
    else {  // this means the only way to go is backwards
        this->direction[0] *= -1;
        this->direction[1] *= -1;
    }
}

void ghostFollowPacman(pGhost this, pPacman pacman, pMap map) {
    int r = rand() % 10;
    // mvprintw(4, 75, "%dfollow r: %d", debug, r);
    if(r == 9) {    // random chance to not follow
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

// bounds checking
void ghostMove(pGhost this, pPacman pacman, pMap map) {
    if(this->trackingPacman) {
        this->trackingPacman = ghostSeesPacman(this, pacman, map, map->elems);
        if(!this->trackingPacman) {
            // lost track
            ghostFollowPacman(this, pacman, map);
        }
        // else continue following with random chance to stop
        else {
            int r = rand() % 20;
            // mvprintw(5, 75, "%d. ghostMove r: %d", debug, r);
            if(r == 19) {
                ghostMoveOptions(this, map, map->elems, TRUE);
                ghostWander(this);
                this->trackingPacman = FALSE;
                this->stopTracking = TRUE;
            }
        }
    }
    else {
        this->trackingPacman = ghostSeesPacman(this, pacman, map, map->elems);
        if(this->trackingPacman) {
            // starts following
            ghostFollowPacman(this, pacman, map);
        }
        else {
            // isn't following
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

    this->x += this->direction[0];
    this->y += this->direction[1];

    if(ghostHitsPacman(this, pacman))
        gameover(pacman);
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