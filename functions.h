#include <stdio.h>
#include <time.h>
#include "pacman.h"

char getInput(clock_t start, int updateRate, pPacman pacman);

void update(pPacman pacman, char c, pMap map);

void draw(pPacman pacman, pMap map);