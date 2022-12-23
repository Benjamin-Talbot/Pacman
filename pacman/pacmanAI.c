#include "pacmanFunctions.h"

// this code was written quickly and hasn't been reviewed
// it is not very clean, it contains some redundant code, and it is not optimized at all

static char prevchars[2] = {' ', ' '};
static char infloopWarning = FALSE;
static char infloop = FALSE;
static int numChoices = 0, numRandTimes = 0;
static int directions[4][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
static char pathfinding = FALSE;
static int randTick = 20;

int randint(int max) {
    int r = rand() % max;
    return r;
}

void pacmanMoveOptions(pPacman this, pMap map, char elems[map->height][map->width], char withBackwards) {
    numChoices = 0;

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

    if(withBackwards) {
        directions[numChoices][0] = this->direction[0] * -1;
        directions[numChoices][1] = this->direction[1] * -1;
        numChoices++;
    }
}

int pacmanSeesPellet(pPacman this, pMap map, char elems[map->height][map->width], int dirs[4][2], int* choice, pPowerup* powerups, int numPowerups) {                         // || == powerup
    int x = this->x + 1, y = this->y, i = 0;
    char foundPellet = FALSE;
    int numCloseishPellets = 0;

    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPellet && withinBounds(x, y, map->width, map->height)) {
        if(elems[y][x] == PELLET)
            foundPellet = TRUE;
        for(i = 0; i < numPowerups && !foundPellet; i++) {
            if(x == powerups[i]->x && y == powerups[i]->y)
                foundPellet = TRUE;
        }
        if(foundPellet) {
            dirs[numCloseishPellets][0] = 1;
            dirs[numCloseishPellets][1] = 0;
            numCloseishPellets++;
            *choice = 0;
        }
        x++;
    }
    x = this->x;
    y = this->y - 1;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPellet && withinBounds(x, y, map->width, map->height)) {
        if(elems[y][x] == PELLET)
            foundPellet = TRUE;
        for(i = 0; i < numPowerups && !foundPellet; i++) {
            if(x == powerups[i]->x && y == powerups[i]->y)
                foundPellet = TRUE;
        }
        if(foundPellet) {
            dirs[numCloseishPellets][0] = 0;
            dirs[numCloseishPellets][1] = -1;
            numCloseishPellets++;
            *choice = 1;
        }
        y--;
    }
    x = this->x - 1;
    y = this->y;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPellet && withinBounds(x, y, map->width, map->height)) {
        if(elems[y][x] == PELLET)
            foundPellet = TRUE;
        for(i = 0; i < numPowerups && !foundPellet; i++) {
            if(x == powerups[i]->x && y == powerups[i]->y)
                foundPellet = TRUE;
        }
        if(foundPellet) {
            dirs[numCloseishPellets][0] = -1;
            dirs[numCloseishPellets][1] = 0;
            numCloseishPellets++;
            *choice = 2;
        }
        x--;
    }
    x = this->x;
    y = this->y + 1;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPellet && withinBounds(x, y, map->width, map->height)) {
        if(elems[y][x] == PELLET)
            foundPellet = TRUE;
        for(i = 0; i < numPowerups && !foundPellet; i++) {
            if(x == powerups[i]->x && y == powerups[i]->y)
                foundPellet = TRUE;
        }
        if(foundPellet) {
            dirs[numCloseishPellets][0] = 0;
            dirs[numCloseishPellets][1] = -1;
            numCloseishPellets++;
            *choice = 3;
        }
        y++;
    }

    return numCloseishPellets;
}

void pelletVector(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, float* pelletxcomp, float* pelletycomp) {
    *pelletxcomp = 0;
    *pelletycomp = 0;

    int numPellets = 0;
    for(int y = 0; y < map->height; y++) {
        for(int x = 0; x < map->width; x++) {
            if(elems[y][x] == PELLET) {
                *pelletxcomp += (x - this->x) / 2;
                *pelletycomp += (y - this->y) / 2;
                numPellets++;
            }
        }
    }

    for(int i = 0; i < numPowerups; i++) {
        *pelletxcomp += (powerups[i]->x - this->x) / 2;
        *pelletycomp += (powerups[i]->y - this->y) / 2;
    }

    *pelletxcomp /= (numPellets + numPowerups) / 2 + 1;
    *pelletycomp /= (numPellets + numPowerups) / 2 + 1;
}

void findNearestPellet(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, int* minx, int* miny) {
    *minx = 2 * (map->width) + 1, *miny = 2 * (map->height) + 1;
    int mind = (this->x - *minx)*(this->x - *minx) + (this->y - *miny)*(this->y - *miny), dx, dy, d;

    for(int y = 0; y < map->height; y++) {
        for(int x = 0; x < map->width; x++) {
            if(elems[y][x] == PELLET) {
                dx = this->x - x;
                dy = this->y - y;
                d = dx*dx + dy*dy;
                if(d < mind) {
                    *minx = x;
                    *miny = y;
                    mind = (this->x - *minx)*(this->x - *minx) + (this->y - *miny)*(this->y - *miny);
                }
            }
        }
    }

    for(int i = 0; i < numPowerups; i++) {
        dx = this->x - powerups[i]->x;
        dy = this->y - powerups[i]->y;
        if(dx*dx + dy*dy < mind) {
            *minx = powerups[i]->x;
            *miny = powerups[i]->y;
            mind = (this->x - *minx)*(this->x - *minx) + (this->y - *miny)*(this->y - *miny);
        }
    }
}

char ghostsInVicinity(pPacman this, pGhost ghosts, int numGhosts, int* minx, int* miny, int* ghostxcomp, int* ghostycomp) {
    char ghostClose = FALSE;
    int minind = 0, mind = 10000;
    int dx, dy, d;
    *ghostxcomp = *ghostycomp = 0;
    Ghost ghost = ghosts[0];

    if(numGhosts > 0) {
        if(!ghost.vulnerable) {
            *minx = ghost.x;
            *miny = ghost.y;
            mind = (*minx)*(*minx) + (*miny)*(*miny);
            
            *ghostxcomp += ghost.x;
            *ghostycomp += ghost.y;
        }

        for(int i = 1; i < numGhosts; i++) {
            ghost = ghosts[i];
            if(!ghost.vulnerable) {
                dx = this->x - ghost.x;
                dy = this->y - ghost.y;
                d = dx*dx + dy*dy;

                *ghostxcomp += ghost.x;
                *ghostycomp += ghost.y;

                if(d < mind) {
                    (*minx) = ghost.x;
                    (*miny) = ghost.y;
                    mind = d;
                    minind = i;
                }
            }
        }

        *ghostxcomp -= ghosts[minind].x;
        *ghostycomp -= ghosts[minind].y;

        if(mind <= 16)
            ghostClose = TRUE;
    }

    return ghostClose;
}

char pacmanChooseDirection(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts, char *c) {
    char ghostClose = FALSE;
    int dirs[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
    int keys[4] = {'d', 'w', 'a', 's'};
    int dirChoices[4];
    int numClosePellets = 0;
    int row, col, i, vx, vy, r;
    int ghostxcomp, ghostycomp;    // vector components for the average ghost distance out of the not closest ghosts
    float pelletxcomp, pelletycomp;    // vector components for the average pellet distance
    int xdir = 0, ydir = 0;
    int minx = 0, miny = 0;

    ghostClose = ghostsInVicinity(this, ghosts, numGhosts, &minx, &miny, &ghostxcomp, &ghostycomp);

    mvprintw(5, 75, "ghostClose: %d", ghostClose);

    if(!ghostClose) {
        for(int i = 0; i < 4; i++) {
            row = this->y + dirs[i][1];
            col = this->x + dirs[i][0];
            if(elems[row][col] == PELLET) {
                dirChoices[numClosePellets] = i;
                numClosePellets++;
            }

            for(int j = 0; j < numPowerups && numClosePellets < 4; j++) {
                if(this->x + dirs[j][0] == powerups[j]->x && this->y + dirs[j][1] == powerups[j]->y) {
                    dirChoices[numClosePellets] = i;
                    numClosePellets++;
                    break;
                }
            }
        }

        if(numClosePellets > 1) {    // go towards ones where higher concentration of dots
                                     // this means finding vector and checking one that is in a closer direction
            infloop = FALSE;
            pelletVector(this, map, elems, powerups, numPowerups, &pelletxcomp, &pelletycomp);
            int i = 0;
            for(i = 0; i < numClosePellets; i++) {
                vx = pelletxcomp + dirs[i][0];
                vy = pelletycomp + dirs[i][1];
                if(abs(vx) <= abs(pelletxcomp) || abs(vy) <= abs(pelletycomp))
                    break;
            }

            r = randint(randTick);
            if((r == randTick - 1) && numClosePellets > 0)
                i = randint(numClosePellets);
            *c = keys[dirChoices[i]];

        }
        else if(numClosePellets == 1) {
            infloop = FALSE;
            // this->direction[0] = dirs[dirChoices[0]][0];
            // this->direction[1] = dirs[dirChoices[0]][1];
            *c = keys[dirChoices[0]];
        }
        else {    // no pellets directly next to pacman
            int choice = 0;
            int numCloseishPellets = pacmanSeesPellet(this, map, elems, dirs, &choice, powerups, numPowerups);

            if(numCloseishPellets == 1) {
                infloop = FALSE;
                // this->direction[0] = dirs[dirChoices[0]][0];
                // this->direction[1] = dirs[dirChoices[0]][1];
                *c = keys[choice];
            }
            else if(numCloseishPellets > 1) {    // find one that is closer to the most pellets
                infloop = FALSE;
                pelletVector(this, map, elems, powerups, numPowerups, &pelletxcomp, &pelletycomp);
                for(int i = 0; i < numCloseishPellets; i++) {
                    vx = pelletxcomp + dirs[i][0];
                    vy = pelletycomp + dirs[i][1];
                    if(abs(vx) <= abs(pelletxcomp) || abs(vy) <= abs(pelletycomp))
                        break;
                }
                r = randint(randTick);
                    if((r == randTick - 1) && numCloseishPellets > 0)
                        i = randint(numCloseishPellets);
                *c = keys[i];
            }
            else {    // numCloseishPellets == 0
                      // go towards highest concentration of pellets

                pelletVector(this, map, elems, powerups, numPowerups, &pelletxcomp, &pelletycomp);
                xdir = pelletxcomp / fabs(pelletxcomp);
                ydir = pelletycomp / fabs(pelletycomp);
                
                // dirs = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
                // keys = {'d', 'w', 'a', 's'};

                // sort in terms of importance (which component is greater)
                if(fabs(pelletxcomp) >= fabs(pelletycomp)) {
                    if(elems[this->y][this->x+xdir] != WALL && elems[this->y][this->x+xdir] != DOOR)
                        if(xdir > 0)
                            *c = keys[0];
                        else
                            *c = keys[2];
                    else if(elems[this->y+ydir][this->x] != WALL && elems[this->y+ydir][this->x] != DOOR)
                        if(ydir > 0)
                            *c = keys[3];
                        else
                            *c = keys[1];
                    else if(elems[this->y-ydir][this->x] != WALL && elems[this->y-ydir][this->x] != DOOR)
                        if(ydir > 0)
                            *c = keys[1];
                        else
                            *c = keys[3];
                    else
                        if(xdir > 0)
                            *c = keys[2];
                        else
                            *c = keys[0];
                }
                else {    // fabs(pelletycomp) > fabs(pelletxcomp)
                    if(elems[this->y+ydir][this->x] != WALL && elems[this->y+ydir][this->x] != DOOR)
                        if(ydir > 0)
                            *c = keys[3];
                        else
                            *c = keys[1];
                    else if(elems[this->y][this->x+xdir] != WALL && elems[this->y][this->x+xdir] != DOOR)
                        if(xdir > 0)
                            *c = keys[0];
                        else
                            *c = keys[2];
                    else if(elems[this->y][this->x-xdir] != WALL && elems[this->y][this->x-xdir] != DOOR)
                        if(xdir > 0)
                            *c = keys[2];
                        else
                            *c = keys[0];
                    else
                        if(xdir > 0)
                            *c = keys[1];
                        else
                            *c = keys[3];
                }
                
                r = randint(randTick);
                if(r == randTick - 1) {
                    pacmanMoveOptions(this, map, elems, FALSE);
                    if(numChoices > 0) {
                        r = randint(numChoices);
                        xdir = directions[r][0];
                        ydir = directions[r][1];
                        if(xdir == 1)
                            *c = keys[0];
                        else if(xdir == -1)
                            *c = keys[2];
                        else if(ydir == -1)
                            *c = keys[1];
                        else    // ydir = 1?
                            *c = keys[3];
                    }
                }

                if(prevchars[0] == *c && prevchars[1] != *c) {
                    if(infloopWarning)
                        infloop = TRUE;
                    infloopWarning = TRUE;
                }
                else
                    infloopWarning = FALSE;

                if(infloop)
                    numRandTimes = 15;
                    // mvprintw(10, 60, "Infinite Loop!");

                prevchars[0] = prevchars[1];
                prevchars[1] = *c;
            }
        }
    }
    else {    // ghostClose == TRUE
        pacmanMoveOptions(this, map , elems, TRUE);
        int rundir[2];
        int x = this->x - minx, y = this->y - miny;
        rundir[0] = (x) / abs(x);
        rundir[1] = (y) / abs(y);

        abs(x) > abs(y) ? (rundir[1] = 0) : (rundir[0] = 0);
        
        if(this->invincible) {
            rundir[0] *= -1;
            rundir[1] *= -1;
        }

        for(i = 0; i < numChoices; i++)
            if(directions[i][0] == rundir[0] && directions[i][1] == rundir[1])
                break;
        if(i == numChoices) {
            rundir[0] *= -1;
            rundir[1] *= -1;
            for(i = 0; i < numChoices; i++)
                if(directions[i][0] != rundir[0] || directions[i][1] != rundir[1])
                    break;
            if(i == numChoices)
                i = 0;
        }
        if(directions[i][0] == 1)
            *c = keys[0];
        else if(directions[i][0] == -1)
            *c = keys[2];
        else if(directions[i][1] == -1)
            *c = keys[1];
        else    // directions[i][1] == 1?
            *c = keys[3];
    }

    if(!ghostClose && infloop && numRandTimes > 0) {    // !ghostClose && infloop
        // if(!pathfinding) {
        //     // findNearestPellet();
        //     pathfinding = TRUE;
        // }
        // // pathfind(x, y);

        pacmanMoveOptions(this, map, elems, FALSE);
        if(numChoices > 0) {
            r = randint(numChoices);
            xdir = directions[r][0];
            ydir = directions[r][1];
            if(xdir == 1)
                *c = keys[0];
            else if(xdir == -1)
                *c = keys[2];
            else if(ydir == -1)
                *c = keys[1];
            else    // ydir == 1?
                *c = keys[3];
        }
        else {
            this->direction[0] *= -1;
            this->direction[1] *= -1;
        }
        numRandTimes--;

        if(numRandTimes == 0)
            infloop = FALSE;
    }

    return *c;
}