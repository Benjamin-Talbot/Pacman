#ifndef GHOST_H
#define GHOST_H

typedef struct Ghost {
    int x;
    int y;
    int initx;
    int inity;
    int direction[2];
    int oldDirection[2];
    int nextDirection[2];
    int turnx;
    int turny;
    char stopTracking;
    char sprite;
    char nextSprite;
    int cooldown;
    char trackingPacman;
    int** directions;
    int numChoices;
    char vulnerable;
    int toggleUpdate;
} Ghost;
typedef Ghost* pGhost;

#endif
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
        else                                                        // dangling else?
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
            else                                                    // dangling else?
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
#ifndef GHOST_FUNCTIONS_H
#define GHOST_FUNCTIONS_H

#include "ghost.h"

#include <ncurses.h>
#include <stdlib.h>

#include "../pacman/pacmanFunctions.h"
#include "../map/mapFunctions.h"

#ifndef WALL
#define WALL '#'    // #, 35
#endif
#ifndef DOOR
#define DOOR '-'    // -, 45
#endif

#undef GO_LEFT
#define GO_LEFT(dir) this->dir[0] = -1; this->dir[1] = 0;
#undef GO_RIGHT
#define GO_RIGHT(dir) this->dir[0] = 1; this->dir[1] = 0;
#undef GO_UP
#define GO_UP(dir) this->dir[0] = 0; this->dir[1] = -1;
#undef GO_DOWN
#define GO_DOWN(dir) this->dir[0] = 0; this->dir[1] = 1;
#undef STOP
#define STOP(dir) this->dir[0] = 0; this->dir[1] = 0;
#undef ALLDIR
#define ALLDIR(dir) this->dir[0] = 1; this->dir[1] = 1;

void ghostInit(pGhost this, int x, int y);

void ghostReset(pGhost this);

char withinBounds(int x, int y, int width, int height);

char ghostSeesPacman(pGhost this, pPacman pacman, pMap map, char elems[map->height][map->width]);

void ghostMoveOptions(pGhost this, pMap map, char elems[map->height][map->width], char withBackwards);

void ghostWander(pGhost this);

void ghostFollowPacman(pGhost this, pPacman pacman, pMap map);

char ghostHitsPacman(pGhost this, pPacman pacman);

char ghostHitsWall(pGhost this, pMap map, char elems[map->height][map->width]);

void ghostRunAway(pGhost this, pMap map, char elems[map->height][map->width], pPacman pacman);

void ghostMove(pGhost this, pPacman pacman, pMap map);

void ghostsMove(pGhost ghosts, int numGhosts, pPacman pacman, pMap map);

void ghostsDraw(pGhost ghosts, int numGhosts, pMap map, char elems[map->height][map->width]);

#endif
#include "mainFunctions.h"

static int paused = FALSE;

char getPlayer() {
    int CPU;
    printf("Would you like to play (1) or let the computer play (0)? ");
    scanf("%d", &CPU);
    CPU = 1 - CPU;
    char garbage;
    scanf("%c", &garbage);
    
    return (char) CPU;
}

// what about empty name? and test if include ':'
// doesn't work with handing ':'
char* getName(char* player) {
    int lenName = 8;
    player = malloc(sizeof(char) * lenName);

    printf("Please enter your name: ");

	int i = 0;
	scanf("%c", &player[0]);
	while(player[i] != '\n' && player[i] != ':') {
		i++;
        if(i >= lenName) {
            lenName *= 2;
            player = realloc(player, lenName);
        }
		scanf("%c", &player[i]);
        // if(player[i] == ':') {
        //     printf("You cannot include the character ':' in your name.\n");
        //     for(int ind = 0; ind < i; ind++)
        //         printf("%c", &player[ind]);
        //     i--;
        // }
	}
    // if(player[i] != ':')
	//     player[i] = '\0';
    // else {
    //     printf("You cannot include the character ':' in your name.\n");
    //     player = getName(player);
    // }

    char* tmp = malloc(sizeof(char) * i);
	strncpy(tmp, player, sizeof(char) * i);
	free(player);

	return tmp;
}

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
    char* elems = malloc(sizeof(char) * (lenElems + 1));
    int maxchars;

    if(level)
        maxchars = sizeof(char)*(18+(int)log10(level) + 1) + 1;
    else    // level is 0, log doesn't work, plus it's constant
        maxchars = 20;
        // maxchars = sizeof(char)*(18+(int)log10(level+1) + 1) + 1;

    char filename[maxchars];
    snprintf(filename, maxchars, "map/levels/map%d.txt", level);

    FILE* mapFile = fopen(filename, "r");
    if(mapFile != NULL) {
        char c = ' ';
        int rows = 0;
        int cols = 0;
        int numchars = 0;

        while(c > 0) {
            cols = 0;
            while(c != '\n' && c > 0) {
                c = fgetc(mapFile);
                if(c > 0 && c != '\n') {
                    elems[numchars] = c;
                    numchars++;
                    if(numchars >= lenElems / sizeof(char)) {
                        lenElems *= 2;
                        elems = realloc(elems, lenElems);
                    }
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
        free(elems);

        replace_char(tmp, ' ', '.', -1);
        replace_char(tmp, 'x', ' ', -1);

        *map = (pMap) malloc(sizeof(Map));
        mapInit(*map, rows, cols, tmp);

        return tmp;
    }

    return NULL;
}

void initialize(pPacman* pacman, char CPU, int score, pMap* map, int level, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts, int pauseTime) {

    loadMap(map, level);

    int** coords = NULL;
    int numpacs = findCoords(&coords, (*map)->height, (*map)->width, (*map)->elems, '<');
    *pacman = (pPacman) malloc(sizeof(Pacman));
    pacmanInit(*pacman, coords[0][0], coords[0][1], score, CPU);

    *numPowerups = malloc(sizeof(int));
    **numPowerups = 0;
    **numPowerups = findCoords(&coords, (*map)->height, (*map)->width, (*map)->elems, 'o');
    *powerups = (pPowerup*) malloc(sizeof(pPowerup) * **numPowerups);
    for(int i = 0; i < **numPowerups; i++) {
        (*powerups)[i] = malloc(sizeof(Powerup));
        powerupInit((*powerups)[i], coords[i][0], coords[i][1]);
    }

    *numGhosts = malloc(sizeof(int));
    **numGhosts = 0;
    **numGhosts = findCoords(&coords, (*map)->height, (*map)->width, (*map)->elems, '&');
    *ghosts = (pGhost) malloc(sizeof(Ghost) * **numGhosts);
    for(int i = 0; i < **numGhosts; i++) {
        ghostInit(*ghosts+i, coords[i][0], coords[i][1]);
    }

    replace_char((*map)->elems, '<', ' ', 1);
    replace_char((*map)->elems, 'o', ' ', -1);
    replace_char((*map)->elems, '&', '.', -1);
    // replace_char((*map)->elems, '&', ' ', -1);

    free(coords[0]);
    free(coords);

    drawWalls(*map, (*map)->elems);
    draw(*pacman, *map, *powerups, **numPowerups, *ghosts, **numGhosts);
    refresh();
    sleep(pauseTime);
}

void freeMemory(pPacman pacman, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int* numGhosts) {
    free(pacman);
    free(map->elems);
    free(map);
    for(int i = 0; i < *numPowerups; i++)
        free(powerups[i]);
    free(powerups);
    free(numPowerups);
    free(ghosts);
    free(numGhosts);
}

void freeScores(pNode node) {
    if(node->left)
        freeScores(node->left);
    if(node->right)
        freeScores(node->right);
    free(node);
}

int nextLevel(pPacman* pacman, pMap* map, int level, int maxLevel, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts, int pauseTime) {
    char CPU = (*pacman)->CPU;
    sleep(pauseTime);
    clearMap(*map, (*map)->elems);
    level++;
    if(level <= maxLevel) {
        int score = (*pacman)->score;
        freeMemory(*pacman, *map, *powerups, *numPowerups, *ghosts, *numGhosts);
        initialize(pacman, CPU, score, map, level, powerups, numPowerups, ghosts, numGhosts, pauseTime);
    }
    else {
        (*pacman)->gameover = TRUE;
    }

    return level;
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
        pacman->changedDirection = FALSE;
    else
        pacman->changedDirection = TRUE;
    
    return c;
}

void update(pPacman pacman, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts) {
    if(c == ' ')
        paused = 1 - paused;

    if(!paused) {
        if(pacman->CPU)
            pacmanChooseDirection(pacman, map, map->elems, powerups, *numPowerups, ghosts, numGhosts, &c);    // make pacman move by itself
        char sprite = pacmanChangeDirection(pacman, c);    // change pacman's direction
        pacmanMove(pacman, sprite, map, powerups, numPowerups, ghosts, numGhosts);    // move pacman
        ghostsMove(ghosts, numGhosts, pacman, map);
    }
}

void draw(pPacman pacman, pMap map, pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts) {
    drawMap(map, map->elems);    // draw the board
    drawWalls(map, map->elems);
    powerupsDraw(powerups, numPowerups);
    ghostsDraw(ghosts, numGhosts, map, map->elems);
    pacmanDraw(pacman);    // print pacman
}

void endGame(int score, char* player) {
    pTree scores = NULL;
    scores = loadScores(scores);
    if(scores) {
        addNode(scores->head, score, player);
    }
    else {
        scores = treeInit(scores, score, player);
    }
    printScores(scores->head);
    writeScores(scores->head);
    freeScores(scores->head);
    free(scores);
    // rank = 1;
    // append = FALSE;
    
    printf("You scored %d points. Good job!\n", score);
    printf("Thanks for playing %s!\n", player);
}
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

void initialize(pPacman* pacman, char CPU, int score, pMap* map, int level, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts, int pauseTime);

void freeMemory(pPacman pacman, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int* numGhosts);

void freeScores(pNode node);

int nextLevel(pPacman* pacman, pMap* map, int level, int maxLevel, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts, int pauseTime);

char getInput(clock_t start, int updateRate, pPacman pacman);

void update(pPacman pacman, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts);

void draw(pPacman pacman, pMap map, pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts);

void endGame(int score, char* player);

#endif
#ifndef MAP_H
#define MAP_H

typedef struct Map {
    int width;
    int height;
    char* elems;
} Map;
typedef Map* pMap;

#endif
#include "mapFunctions.h"

pMap mapInit(pMap this, int rows, int cols, char* elems) {
    this->width = cols;
    this->height = rows;
    this->elems = elems;
    return this;
}

void eatPellet(pMap this, char elems[this->height][this->width], int x, int y) {
    elems[y][x] = ' ';
}

void drawWalls(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(elems[r][c] == WALL || elems[r][c] == DOOR) {
                mvprintw(r, c, "%c", elems[r][c]);
            }
        }
    }
}

void drawMap(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            if(elems[r][c] == PELLET || elems[r][c] == SPACE) {
                mvprintw(r, c, "%c", elems[r][c]);
            }
        }
    }
}

void clearMap(pMap this, char elems[this->height][this->width]) {
    for(int r = 0; r < this->height; r++) {
        for(int c = 0; c < this->width; c++) {
            mvprintw(r, c, " ");
        }
    }
}
#ifndef MAP_FUNCTIONS_H
#define MAP_FUNCTIONS_H

#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>

#ifndef WALL
#define WALL '#'    // 35
#endif
#ifndef DOOR
#define DOOR '-'    // 35
#endif
#ifndef PELLET
#define PELLET '.'  // 46
#endif
#ifndef SPACE
#define SPACE ' '   // 32
#endif

pMap mapInit(pMap this, int rows, int cols, char* elems);

void eatPellet(pMap this, char elems[this->height][this->width], int x, int y);

void drawWalls(pMap this, char elems[this->height][this->width]);

void drawMap(pMap this, char elems[this->height][this->width]);

void clearMap(pMap this, char elems[this->height][this->width]);

#endif
#ifndef PACMAN_H
#define PACMAN_H

typedef struct Pacman {
    int x;
    int y;
    int direction[2];
    int oldDirection[2];
    int nextDirection[2];
    char changedDirection;
    char sprite;
    char nextSprite;
    int score;
    int ghostPoints;
    int invincible;
    char gameover;
    char won;
    char CPU;
} Pacman;
typedef Pacman* pPacman;

#endif
#include "pacmanFunctions.h"

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
#include "pacmanFunctions.h"

void pacmanInit(pPacman this, int x, int y, int score, char CPU) {
    this->x = x;
    this->y = y;
    STOP(direction);
    STOP(oldDirection);
    STOP(nextDirection);
    this->changedDirection = FALSE;
    this->sprite = 60;
    this->nextSprite = 60;
    this->score = score;
    this->ghostPoints = 200;
    this->invincible = 0;
    this->gameover = FALSE;
    this->won = FALSE;
    this->CPU = CPU;
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

void pacmanMakeInvincible(pPacman this, pGhost ghosts, int numGhosts) {
    this->invincible = 40;
    this->ghostPoints = 200;
    for(int i = 0; i < numGhosts; i++) {
        ghosts[i].vulnerable = TRUE;
    }
}

void increaseScore(pPacman this, int points) {
    this->score += points;
}

int pacmanEat(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts) {
    char ate = FALSE;
    char allGone = TRUE;
    if(elems[this->y][this->x] == PELLET) {
        increaseScore(this, 10);
        eatPellet(map, map->elems, this->x, this->y);
        ate = TRUE;

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
                pacmanMakeInvincible(this, ghosts, numGhosts);
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

void pacmanEatGhost(pPacman this, pGhost ghost) {
    if(ghost->vulnerable == TRUE) {
        increaseScore(this, this->ghostPoints);
        ghostReset(ghost);
        this->ghostPoints *= 2;
    }
    else
        gameover(this);
}

char pacmanHitsGhost(pPacman this, pGhost ghosts, int numGhosts) {
    char hitGhost = FALSE;
    pGhost ghost;
    for(int i = 0; i < numGhosts && !hitGhost; i++) {
        ghost = (ghosts+i);

        if(this->x == ghost->x && this->y == ghost->y) {
            if(!ghost->cooldown) {
                hitGhost = TRUE;
                if(this->invincible)
                    pacmanEatGhost(this, ghost);
            }
        }
    }

    return hitGhost;
}

void pacmanUninvincible(pGhost ghosts, int numGhosts) {
    for(int i = 0; i < numGhosts; i++) {
        ghosts[i].vulnerable = FALSE;
        ghosts[i].toggleUpdate = 1;
    }
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
    
    if(this->invincible > 0) {
        (this->invincible)--;
        if(this->invincible <= 0)
            pacmanUninvincible(ghosts, numGhosts);
    }

    pacmanEat(this, map, map->elems, powerups, numPowerups, ghosts, numGhosts);
    if(pacmanHitsGhost(this, ghosts, numGhosts))
        if(!this->invincible)
            gameover(this);
}

void pacmanDraw(pPacman this) {
    mvprintw(this->y, this->x, "%c", this->sprite);    // draw pacman at new position
    mvprintw(5, 60, "Score: %d", this->score);    // update score
}
#ifndef PACMAN_FUNCTIONS_H
#define PACMAN_FUNCTIONS_H

#include "pacman.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>

#include "../map/mapFunctions.h"
#include "../powerup/powerupFunctions.h"
#include "../ghost/ghostFunctions.h"

#ifndef WALL
#define WALL ('#')    // #, 35
#endif
#ifndef DOOR
#define DOOR ('-')    // -, 45
#endif
#ifndef PELLET
#define PELLET ('.')    // ., 46
#endif

#undef GO_LEFT
#define GO_LEFT(dir) this->dir[0] = -1; this->dir[1] = 0;
#undef GO_RIGHT
#define GO_RIGHT(dir) this->dir[0] = 1; this->dir[1] = 0;
#undef GO_UP
#define GO_UP(dir) this->dir[0] = 0; this->dir[1] = -1;
#undef GO_DOWN
#define GO_DOWN(dir) this->dir[0] = 0; this->dir[1] = 1;
#undef STOP
#define STOP(dir) this->dir[0] = 0; this->dir[1] = 0;

// begin pacman AI functions
int randint(int max);

void pacmanMoveOptions(pPacman this, pMap map, char elems[map->height][map->width], char withBackwards);

int pacmanSeesPellet(pPacman this, pMap map, char elems[map->height][map->width], int dirs[4][2], int* choice, pPowerup* powerups, int numPowerups);

void pelletVector(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, float* pelletxcomp, float* pelletycomp);

void findNearestPellet(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, int* minx, int* miny);

char ghostsInVicinity(pPacman this, pGhost ghosts, int numGhosts, int* minx, int* miny, int* ghostxcomp, int* ghostycomp);

char pacmanChooseDirection(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts, char *c);
// end pacman AI functions

void pacmanInit(pPacman this, int x, int y, int score, char CPU);

void gameover(pPacman this);

char pacmanChangeDirection(pPacman this, char c);

int pacmanCollides(pPacman player, pMap map, char elems[map->height][map->width]);

void pacmanMakeInvincible(pPacman this, pGhost ghosts, int numGhosts);

void increaseScore(pPacman this, int points);

int pacmanEat(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts);

void pacmanEatGhost(pPacman this, pGhost ghost);

char pacmanHitsGhost(pPacman this, pGhost ghosts, int numGhosts);

void pacmanUninvincible(pGhost ghosts, int numGhosts);

// char pacmanChooseDirection(pPacman this, pMap map, char elems[map->height][map->width], pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts, char *c);

void pacmanMove(pPacman this, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts);

void pacmanDraw(pPacman this);

#endif
#ifndef POWERUP_H
#define POWERUP_H

typedef struct Powerup {
    int x;
    int y;
    char sprites[4];
    int sprite;
    int numSprites;
} Powerup;
typedef Powerup* pPowerup;

#endif
#include "powerupFunctions.h"

void powerupInit(pPowerup this, int x, int y) {
    this->x = x;
    this->y = y;
    this->sprites[0] = '-';
    this->sprites[1] = '\\';
    this->sprites[2] = '|';
    this->sprites[3] = '/';
    this->sprite = 0;
    this->numSprites = 4;
}

void powerupDelete(pPowerup* powerups, int* numPowerups, int index) {
    free(powerups[index]);
    for(int i = index; i < *numPowerups - 1; i++) {
        powerups[i] = powerups[i+1];
    }
    (*numPowerups)--;
}

void powerupsDraw(pPowerup* powerups, int numPowerups) {
    for(int i = 0; i < numPowerups; i++) {
        mvprintw(powerups[i]->y, powerups[i]->x, "%c", powerups[i]->sprites[powerups[i]->sprite]);
        powerups[i]->sprite++;
        if(powerups[i]->sprite >= powerups[i]->numSprites)
            powerups[i]->sprite = 0;
    }
}
#ifndef POWERUP_FUNCTIONS_H
#define POWERUP_FUNCTIONS_H

#include "powerup.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void powerupInit(pPowerup this, int x, int y);

void powerupDelete(pPowerup* powerups, int* numPowerups, int index);

void powerupsDraw(pPowerup* powerups, int numPowerups);

#endif
#include "score.h"

pNode nodeInit(int score, char* name) {
    pNode node = (pNode) malloc(sizeof(Node));
    node->score = score;
    node->name = name;
    node->left = NULL;
    node->right = NULL;

    return node;
}

pTree treeInit(pTree tree, int score, char* name) {
    pNode node = nodeInit(score, name);
    tree = (pTree) malloc(sizeof(Tree));
    tree->head = node;

    return tree;
}

void addNode(pNode head, int score, char* name) {
    if(score <= head->score) {
        if(head->left) {
            addNode(head->left, score, name);
        }
        else
            head->left = nodeInit(score, name);
    }
    else {
        if(head->right) {
            addNode(head->right, score, name);
        }
        else
            head->right = nodeInit(score, name);
    }
}

pTree loadScores(pTree scores) {
    scores = NULL;

    FILE* scoresFile = fopen("score/scores.txt", "r");
    char c = ' ';
    int lenName = 8;
    char* name;
    int numchars = 0;
    int score = 0;
    
    while(c > 0) {
        lenName = 8;
        name = (char*) malloc(sizeof(char) * lenName);
        numchars = 0;   // was this it??? //
        while(c != ':' && c > 0){
            c = fgetc(scoresFile);
            if(c > 0 && c != ':') {
                name[numchars] = c;
                numchars++;
                if(numchars >= lenName / sizeof(char)) {
                    lenName *= 2;
                    name = realloc(name, lenName);
                }
            }
        }
        name[numchars] = '\0';
        numchars++;
        name = realloc(name, sizeof(char) * numchars);
        fscanf(scoresFile, "%d", &score);
        if(c > 0) {
            if(scores) {
                addNode(scores->head, score, name);
            }
            else {
                scores = treeInit(scores, score, name);
            }
        }
        c = fgetc(scoresFile);
        if(c > 0 && c == '\n') {
        }
    }

    fclose(scoresFile);

    return scores;
}

void writeScore(pNode score) {
    FILE* scoresFile;
    if(append)
        scoresFile = fopen("score/scores.txt", "a");
    else {
        scoresFile = fopen("score/scores.txt", "w");
        append = TRUE;
    }
    
    if(numWritten < rank - 1)
        fprintf(scoresFile, "%s:%d\n", score->name, score->score);
    else
        fprintf(scoresFile, "%s:%d", score->name, score->score);
    numWritten++;

    fclose(scoresFile);
}

void writeScores(pNode head) {
    if(head->left)
        writeScores(head->left);
    writeScore(head);
    if(head->right)
        writeScores(head->right);
}

void printScores(pNode head) {
    if(head->right)
        printScores(head->right);
    printf("%d.\t%s: %d\n", rank, head->name, head->score);
    rank++;
    if(head->left)
        printScores(head->left);
}

void printTree(pNode head, int level) {
    if(head->right) {
        printTree(head->right, level + 1);
    }
    for(int i = 0; i < level; i++)
        printf("    ");
    printf("%s: %d\n", head->name, head->score);
    if(head->left)
        printTree(head->left, level + 1);
}
#ifndef SCORE_H
#define SCORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef struct Node {
    int score;
    char* name;
    struct Node* left;
    struct Node* right;
} Node;
typedef Node* pNode;

typedef struct Tree {
    pNode head;
} Tree;
typedef Tree* pTree;

static int rank = 1;
static int numWritten = 1;
static int append = FALSE;

pNode nodeInit(int score, char* name);

pTree treeInit(pTree tree, int score, char* name);

void addNode(pNode head, int score, char* name);

pTree loadScores(pTree scores);

void writeScores(pNode head);

void printScores(pNode head);

void printTree(pNode head, int level);

#endif
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#include "pacman/pacmanFunctions.h"
#include "map/mapFunctions.h"
#include "powerup/powerupFunctions.h"
#include "ghost/ghostFunctions.h"
#include "mainFunctions/mainFunctions.h"
#include "score/score.h"

int main() {
    char CPU;
    CPU = getPlayer();
    char* player = "CPU";
    if(!CPU)
        player = getName(player);
    fflush(stdout);

    srand(time(NULL));
    clock_t start;
    int updateRate = 150;
    int pauseTime = 1;
    char c = '\0';
    pPacman pacman = NULL;
    pMap map = NULL;
    int maxLevel = 2;
    int level = 0; //level++; //level++;
    pPowerup* powerups = NULL;
    int* numPowerups = NULL;
    pGhost ghosts = NULL;
    int* numGhosts = NULL;

    initscr();
    curs_set(0);
    nodelay(stdscr, true);
    noecho();
    
    initialize(&pacman, CPU, 0, &map, level, &powerups, &numPowerups, &ghosts, &numGhosts, pauseTime);

    while(!pacman->gameover) {
        start = clock();

        if(pacman->won)
            level = nextLevel(&pacman, &map, level, maxLevel, &powerups, &numPowerups, &ghosts, &numGhosts, pauseTime);

        if(!pacman->gameover) {
            clear();
            update(pacman, c, map, powerups, numPowerups, ghosts, *numGhosts);
            draw(pacman, map, powerups, *numPowerups, ghosts, *numGhosts);
        }

        c = getInput(start, updateRate, pacman);
    }
    sleep(pauseTime);
    endwin();

    endGame(pacman->score, player);
    freeMemory(pacman, map, powerups, numPowerups, ghosts, numGhosts);
    if(!CPU)
        free(player);

    return 0;
}

/*
cd C/FinalProject; make clean; make main; clear; ./main
cd C/FinalProject; make clean; make main; clear; lldb main
*/