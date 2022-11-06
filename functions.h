#include <stdio.h>
#include <time.h>
#include "pacman.h"

char getInput(clock_t start, int updateRate);

void update(pPacman pacman, char c, int rows, int cols, char* elems);

void draw(pPacman pacman, int rows, int cols, char* elems);