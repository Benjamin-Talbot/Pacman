#include "functions.h"

char getInput(clock_t start, int updateRate, pPacman pacman) {
    char nextc = getch();
    char c = ERR;

    while(nextc != ERR) {
        c = nextc;
        nextc = getch();
    }

    clock_t end = clock();
    long time_elapsed = (long) ((end - start)/(double)CLOCKS_PER_SEC*1000);

    while(time_elapsed < updateRate) {
        nextc = getch();
        if(nextc != ERR) {
            c = nextc;
        }
        end = clock();
        time_elapsed = (long) ((end - start)/(double)CLOCKS_PER_SEC*1000);
    }

    if(c == ERR)
        pacman->changedDirection = 0;
    else
        pacman->changedDirection = 1;
    
    return c;
}

void update(pPacman pacman, char c, pMap map) {
    char sprite = pacmanChangeDirection(pacman, c);    // change pacman's direction
    pacmanMove(pacman, sprite, map);    // move pacman
    mvprintw(5, 60, "Score: %d", pacman->score);    // update score
}

void draw(pPacman pacman, pMap map, Powerup powerups[]) {
    drawMap(map, map->elems);    // draw the board
    // int numPowerups = sizeof(powerups);
    // if(numPowerups)
    //     numPowerups /= sizeof(powerups[0]);
    // for(int i = 0; i < numPowerups; i++)
    powerupDraw(powerups);
    pacmanDraw(pacman);    // print pacman
}