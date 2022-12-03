#include "pacmanFunctions.h"

void pacmanInit(pPacman this, int x, int y, int score) {
    this->x = x;
    this->y = y;
    STOP(direction);
    STOP(oldDirection);
    STOP(nextDirection);
    this->changedDirection = FALSE;
    this->sprite = 60;
    this->nextSprite = 60;
    this->score = score;
    this->invincible = 0;
    this->gameover = FALSE;
    this->won = FALSE;
}

void gameover(pPacman this) {
    this->gameover = TRUE;
}

char pacmanChangeDirection(pPacman this, char c) {
    char sprite = this->sprite;
    this->oldDirection[0] = this->direction[0];
    this->oldDirection[1] = this->direction[1];
    switch(c) {
            case 119:   // w, move up
                GO_UP(direction);
                sprite = 118;   // v
                break;
            case 97:    // a, go left
                GO_LEFT(direction);
                sprite = 62;    // >
                break;
            case 115:   // s, go down
                GO_DOWN(direction);
                sprite = 94;    // ^
                break;
            case 100:   // d, go right
                GO_RIGHT(direction);
                sprite = 60;    // <
                break;
    }
    
    return sprite;
}

int pacmanCollides(pPacman this, pMap map, char elems[map->height][map->width]) {
    if(this->x >= 0 && this->x < map->width && this->y >= 0 && this->y < map->height)
        return elems[this->y][this->x] == WALL || elems[this->y][this->x] == DOOR ? TRUE : FALSE;
    return FALSE;
}

void pacmanMakeInvincible(pPacman this) {
    this->invincible = 54;
}

void increaseScore(pPacman this, int points) {
    this->score += points;
}

int pacmanEat(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int* numPowerups) {
    char ate = FALSE;
    char allGone = TRUE;
    if(elems[this->y][this->x] == PELLET) {
        increaseScore(this, 10);
        eatPellet(map, map->elems, this->x, this->y);
        ate = TRUE;
        // elems[this->y][this->x] = ' ';    // should this be in map?

        for(int row = 0; row < map->height && allGone; row++)
            for(int col = 0; col < map->width && allGone; col++)
                if (elems[row][col] == PELLET)
                    allGone = FALSE;
        if(allGone && *numPowerups == 0)
            this->won = TRUE;
    }
    else {
        for(int i = 0; i < *numPowerups; i++) {
            if(this->x == powerups[i]->x && this->y == powerups[i]->y) {
                increaseScore(this, 50);
                pacmanMakeInvincible(this);
                powerupDelete(powerups, numPowerups, i);
                for(int row = 0; row < map->height && allGone; row++)
                    for(int col = 0; col < map->width && allGone; col++)
                        if (elems[row][col] == PELLET)
                            allGone = FALSE;
                if(allGone && *numPowerups == 0)
                    this->won = TRUE;
            }
        }
    }
    return ate;
}

char pacmanHitsGhost(pPacman this, pGhost ghosts, int numGhosts) {
    char hitGhost = FALSE;
    pGhost ghost;
    for(int i = 0; i < numGhosts && !hitGhost; i++) {
        ghost = (ghosts+i);

        if(this->x == ghost->x && this->y == ghost->y) {
            hitGhost = TRUE;
        }
    }

    return hitGhost;
}

void pacmanMove(pPacman this, char sprite, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts) {
    int oldx = this->x, oldy = this->y;

    if(this->changedDirection || (!this->nextDirection[0] && !this->nextDirection[1])) {
        if(this->changedDirection) {
            STOP(nextDirection);
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
                STOP(direction);
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
            STOP(nextDirection);
            this->changedDirection = 1;
        }
        else {
            this->x = oldx + this->oldDirection[0];
            this->y = oldy + this->oldDirection[1];

            if(pacmanCollides(this, map, map->elems)) {
                this->x = oldx;
                this->y = oldy;
                STOP(direction);
                STOP(nextDirection);
            }
        }
    }
    
    if(this->invincible > 0)
        (this->invincible)--;
    

    pacmanEat(this, map, map->elems, powerups, numPowerups);
    if(pacmanHitsGhost(this, ghosts, numGhosts))
        gameover(this);
}

void pacmanDraw(pPacman this) {
    mvprintw(this->y - this->direction[1], this->x - this->direction[0], " ");    // erase pacman from previous position
    mvprintw(this->y, this->x, "%c", this->sprite);    // draw pacman at new position
    mvprintw(8, 60, "Invincible: %d", this->invincible);
    mvprintw(5, 60, "Score: %d", this->score);    // update score
}