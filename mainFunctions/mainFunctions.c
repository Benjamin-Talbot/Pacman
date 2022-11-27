#include "mainFunctions.h"

void replace_char(char str[], char oldc, char newc, int max) {
    for(int i = 0; str[i] && max;) {
        if(str[i] == oldc) {
            str[i] = newc;
            if(max > 0)
                max--;
        }
        else
            i++;
    }
}

int findCoords(int*** coords, int rows, int cols, char elems[rows][cols], char character) {
    // holds the max number of coordinates (x,y pairs) in coords
    int sizeCoords = sizeof(int) * 2 * 4;
    // holds coordinates as a pointer-referenced 2D array
    if(*coords != NULL) {
        free(*coords);
    }
    *coords = malloc(sizeof(int) * sizeCoords);
    (*coords)[0] = (int*) malloc(sizeof(int) * sizeCoords * 2);
    // index to copy coordinates into (*coords)[], also twice the current number of coordinates (so # x's + # y's)
    int index = 0;

    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(elems[r][c] == character) {
                if(index >= sizeCoords) {
                    sizeCoords *= 2;
                    (*coords)[0] = realloc((*coords)[0], sizeof(int) * sizeCoords * 2);
                }
                (*coords)[0][index] = c;
                (*coords)[0][index+1] = r;
                index += 2;
            }
        }
    }

    (*coords)[0] = realloc((*coords)[0], sizeof(int) * index);
    index /= 2;

    for(int i = 0; i < index; i++) {
        (*coords)[i] = &(*coords)[0][i*2];
    }

    return index;
}

char* loadMap(pMap* map, int level) {
    int lenElems = 500;
    char* elems = malloc(sizeof(char) * lenElems);
    int maxchars;

    // cite the log expression
    if(level)
        maxchars = sizeof(char)*(18+(int)log10(level) + 1) + 1;
    else
        maxchars = sizeof(char)*(18+(int)log10(level+1) + 1) + 1;

    char filename[maxchars];
    snprintf(filename, maxchars, "map/levels/map%d.txt", level);

    FILE* mapFile = fopen(filename, "r");
    char c = ' ';
    int rows = 0;
    int cols = 0;
    int numchars = 0;
    int success = 1;

    while(success > 0) {
        cols = 0;
        while(c != '\n' && success > 0) {
            success = fscanf(mapFile, "%c", &c);
            if(success > 0 && c != '\n') {
                numchars++;
                if(numchars >= lenElems / sizeof(char)) {
                    lenElems *= 2;
                    elems = realloc(elems, lenElems);
                }
                strncat(elems, &c, 1);
                cols++;
            }
        }
        rows++;
        if(c == '\n')
            c = ' ';
    }
    fclose(mapFile);

    char* tmp = malloc(sizeof(char) * numchars);
    strncpy(tmp, elems, numchars);

    replace_char(tmp, ' ', '.', -1);
    replace_char(tmp, 'x', ' ', -1);

    *map = (pMap) malloc(sizeof(Map));
    mapInit(*map, rows, cols, tmp);

    return tmp;
}

void initialize(pPacman* pacman, pMap* map, int level, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts) {

    loadMap(map, level);

    int** coords = NULL;
    int numpacs = findCoords(&coords, (*map)->height, (*map)->width, (*map)->elems, '<');
    *pacman = (pPacman) malloc(sizeof(Pacman));
    pacmanInit(*pacman, coords[0][0], coords[0][1]);

    *numPowerups = malloc(sizeof(int));
    **numPowerups = findCoords(&coords, (*map)->height, (*map)->width, (*map)->elems, 'o');
    *powerups = (pPowerup*) malloc(sizeof(pPowerup) * **numPowerups);
    for(int i = 0; i < **numPowerups; i++) {
        (*powerups)[i] = malloc(sizeof(Powerup));
        powerupInit((*powerups)[i], coords[i][0], coords[i][1]);
    }

    *numGhosts = malloc(sizeof(int));
    **numGhosts = findCoords(&coords, (*map)->height, (*map)->width, (*map)->elems, '&');
    *ghosts = (pGhost) malloc(sizeof(Ghost) * **numGhosts);
    for(int i = 0; i < **numGhosts; i++) {
        ghostInit(*ghosts+i, coords[i][0], coords[i][1]);
    }

    replace_char((*map)->elems, '<', ' ', 1);
    replace_char((*map)->elems, 'o', ' ', -1);
    replace_char((*map)->elems, '&', '.', 1);
    replace_char((*map)->elems, '&', ' ', -1);
}

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

void update(pPacman pacman, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts) {
    char sprite = pacmanChangeDirection(pacman, c);    // change pacman's direction
    pacmanMove(pacman, sprite, map, powerups, numPowerups, ghosts, numGhosts);    // move pacman
    ghostsMove(ghosts, numGhosts, pacman, map);
}

void draw(pPacman pacman, pMap map, pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts) {
    drawMap(map, map->elems);    // draw the board
    powerupsDraw(powerups, numPowerups);
    ghostsDraw(ghosts, numGhosts, map, map->elems);
    pacmanDraw(pacman);    // print pacman
}