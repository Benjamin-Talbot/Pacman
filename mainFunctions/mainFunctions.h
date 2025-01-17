#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "../pacman/pacmanFunctions.h"
#include "../map/mapFunctions.h"
#include "../powerup/powerupFunctions.h"
#include "../ghost/ghostFunctions.h"
#include "../score/score.h"

char getPlayer();
char* getName(char* player);
void replace_char(char str[], char oldc, char newc, int max);
int findCoords(int*** coords, int rows, int cols, char elems[rows][cols], char character);
char* loadMap(pMap* map, int level);
void initialize(pPacman* pacman, char CPU, int score, pMap* map, int level, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts, pPortal** portals, int* numPortals, int pauseTime);
void freeMemory(pPacman pacman, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int* numGhosts);
void freeScores(pNode node);
int nextLevel(pPacman* pacman, pMap* map, int level, int maxLevel, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts, pPortal** portals, int* numPortals, int pauseTime);
char getInput(clock_t start, int updateRate, pPacman pacman);
void update(pPacman pacman, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts, pPortal* portals, int numPortals);
void draw(pPacman pacman, pMap map, pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts, pPortal* portals, int numPortals);
void endGame(int score, char* player);

#endif