#include "pacmanFunctions.h"

int pacmanSeesPellet(pPacman this, pMap map, char elems[map->height][map->width], int dirs[4][2]) {
    int x = this->x + 1;
    int y = this->y;
    char foundPellet = FALSE;
    int numCloseishPellets = 0;

    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPellet && withinBounds(x, y, map->width, map->height)) {
        if(elems[y][x] == PELLET) {
            foundPellet = TRUE;
            dirs[numCloseishPellets][0] = 1;
            dirs[numCloseishPellets][1] = 0;
            numCloseishPellets++;
        }
        x++;
    }
    x = this->x;
    y = this->y - 1;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPellet && withinBounds(x, y, map->width, map->height)) {
        if(elems[y][x] == PELLET) {
            foundPellet = TRUE;
            dirs[numCloseishPellets][0] = 0;
            dirs[numCloseishPellets][1] = -1;
            numCloseishPellets++;
        }
        y--;
    }
    x = this->x - 1;
    y = this->y;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPellet && withinBounds(x, y, map->width, map->height)) {
        if(elems[y][x] == PELLET) {
            foundPellet = TRUE;
            dirs[numCloseishPellets][0] = -1;
            dirs[numCloseishPellets][1] = 0;
            numCloseishPellets++;
        }
        x--;
    }
    x = this->x;
    y = this->y + 1;
    while(elems[y][x] != WALL && elems[y][x] != DOOR && !foundPellet && withinBounds(x, y, map->width, map->height)) {
        if(elems[y][x] == PELLET) {
            foundPellet = TRUE;
            dirs[numCloseishPellets][0] = 0;
            dirs[numCloseishPellets][1] = -1;
            numCloseishPellets++;
        }
        y++;
    }

    return numCloseishPellets;
}

void pelletVector(pPacman this, pMap map, char elems[map->height][map->width], Powerup* powerups, int numPowerups, float* pelletxcomp, float* pelletycomp) {
    *pelletxcomp = 0;
    *pelletycomp = 0;

    int numPellets = 0;
    for(int y = 0; y < map->height; y++) {
        for(int x = 0; x < map->width; x++) {
            if(elems[y][x] == PELLET) {
                *pelletxcomp += x - this->x;
                *pelletycomp += y - this->y;
                numPellets++;
            }
        }
    }

    for(int i = 0; i < numPowerups; i++) {
        *pelletxcomp += powerups[i].x - this->x;
        *pelletycomp += powerups[i].y - this->y;
    }

    // if not 0

    *pelletxcomp /= numPellets + numPowerups;
    *pelletycomp /= numPellets + numPowerups;
}

void ghostVector() {

}

char pacmanChooseDirection(pPacman this, pMap map, char elems[map->height][map->width], Powerup* powerups, int numPowerups, pGhost ghosts, int numGhosts, char *c) {
    char ghostClose = FALSE;
    int dirs[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
    int keys[4] = {'d', 'w', 'a', 's'};
    int dirChoices[4];
    int numClosePellets = 0;
    int row, col, i, vx, vy;
    float pelletxcomp, pelletycomp;    // vector components for the average pellet distance

    if(!ghostClose) {
        for(int i = 0; i < 4; i++) {
            row = this->y + dirs[i][1];
            col = this->x + dirs[i][0];
            if(elems[row][col] == PELLET) {
                dirChoices[numClosePellets] = i;
                numClosePellets++;
            }
        }

        if(numClosePellets > 1) {    // go towards ones where higher concentration of dots
                                     // this means finding vector and checking one that is in a closer direction
            pelletVector(this, map, elems, powerups, numPowerups, &pelletxcomp, &pelletycomp);
            i = 0;
            for(i = 0; i < numClosePellets; i++) {
                vx = pelletxcomp + dirs[i][0];
                vy = pelletycomp + dirs[i][1];
                if(abs(vx) < abs(pelletxcomp) || abs(vy) < abs(pelletycomp))
                    break;
            }
            *c = keys[i];
        }
        else if(numClosePellets == 1) {
            // this->direction[0] = dirs[dirChoices[0]][0];
            // this->direction[1] = dirs[dirChoices[0]][1];
            *c = keys[0];
        }
        else {    // no pellets directly next to pacman
            int numCloseishPellets = pacmanSeesPellet(this, map, elems, dirs);

            if(numCloseishPellets == 1) {
                // this->direction[0] = dirs[dirChoices[0]][0];
                // this->direction[1] = dirs[dirChoices[0]][1];
                *c = keys[dirChoices[0]];
            }
            else if(numCloseishPellets > 1) {    // find one that is closer to the most pellets
                pelletVector(this, map, elems, powerups, numPowerups, &pelletxcomp, &pelletycomp);
                for(int i = 0; i < numCloseishPellets; i++) {
                    vx = pelletxcomp + dirs[i][0];
                    vy = pelletycomp + dirs[i][1];
                    if(abs(vx) < abs(pelletxcomp) || abs(vy) < abs(pelletycomp))
                        break;
                }
                *c = keys[i];
            }
            else {    // numCloseishPellets == 0
                      // go towards highest concentration of pellets

                int xdir = 0, ydir = 0;

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
            }
        }
    }

    return *c;
}