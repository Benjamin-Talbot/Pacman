#include "mainFunctions.h"

// change ghosts from array of ghost structs to array of ghost pointers

static int paused = FALSE;

// gets the boolean value of whether the player or the CPU will play
// making sure the correct input is accepted has not been implemented yet
char getPlayer() {
    int CPU;
    printf("Would you like to play (1) or let the computer play (0)? ");
    scanf("%d", &CPU);
    CPU = 1 - CPU;
    char garbage;
    scanf("%c", &garbage);
    
    return (char) CPU;
}

// gets the player's name
// what about empty name? and test if include ':'
// doesn't work with handling ':'
// still need to complete this function with input checking
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

// replaces every instance of the given character oldc with the new character newc, a maximum of max instances
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

// finds the coordinates of the given character in the map's elements, as many times as it appears
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

// loads the map from a text file into an array, then initializes a map instance with this array as its elements
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

    // read each character into the array, organized by row
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

// initializes all the main variables used while playing the game
void initialize(pPacman* pacman, char CPU, int score, pMap* map, int level, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts, pPortal** portals, int* numPortals, int pauseTime) {

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

    *numPortals = 0;
    *numPortals = findCoords(&coords, (*map)->height, (*map)->width, (*map)->elems, 'O');
    *portals = (pPortal*) malloc(sizeof(pPortal) * *numPortals);
    for(int i = 0; i < *numPortals; i++) {
        (*portals)[i] = malloc(sizeof(Portal));
        portalInit((*portals)[i], coords[i][0], coords[i][1]);
    }
    for(int i = 0; i < *numPortals; i += 2) {
        portalLink((*portals)[i], (*portals)[i+1]);
    }

    replace_char((*map)->elems, '<', ' ', 1);
    replace_char((*map)->elems, 'o', ' ', -1);
    replace_char((*map)->elems, '&', '.', -1);
    replace_char((*map)->elems, 'O', ' ', -1);

    free(coords[0]);
    free(coords);

    drawWalls(*map, (*map)->elems);
    draw(*pacman, *map, *powerups, **numPowerups, *ghosts, **numGhosts, *portals, *numPortals);
    refresh();
    sleep(pauseTime);
}

// free's the memory used by the main variables used while playing the game
// still need to free portals and numPortals
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
    // free(portals);
    // free(numPortals);
}

// traverse the binary search tree of the scores and free all the memory used
void freeScores(pNode node) {
    if(node->left)
        freeScores(node->left);
    if(node->right)
        freeScores(node->right);
    free(node);
}

// used to reinitialize all the variables to load the next level
// could reduce the overhead by simply changing what needs to be changed instead of freeing and reinitializing everything,
//   but the overhead is not significant in terms of the player noticing so this can be fixed later
int nextLevel(pPacman* pacman, pMap* map, int level, int maxLevel, pPowerup** powerups, int** numPowerups, pGhost* ghosts, int** numGhosts, pPortal** portals, int* numPortals, int pauseTime) {
    char CPU = (*pacman)->CPU;
    sleep(pauseTime);
    clearMap(*map, (*map)->elems);
    level++;
    if(level <= maxLevel) {
        int score = (*pacman)->score;
        freeMemory(*pacman, *map, *powerups, *numPowerups, *ghosts, *numGhosts);
        initialize(pacman, CPU, score, map, level, powerups, numPowerups, ghosts, numGhosts, portals, numPortals, pauseTime);
    }
    else {
        (*pacman)->gameover = TRUE;
    }

    return level;
}

// used to get the character to move Pacman
char getInput(clock_t start, int updateRate, pPacman pacman) {
    char nextc = getch();
    char c = ERR;

    // empties the input buffer to get the last one
    // could probably flush it instead
    while(nextc != ERR) {
        c = nextc;
        nextc = getch();
    }

    clock_t end = clock();
    long time_elapsed = (long) ((end - start)/(double)CLOCKS_PER_SEC*1000);

    // wait until a specified amount of time has passed to keep the update rate of the game constant
    //   and to only get a single character per update
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

// updates the state of the game
void update(pPacman pacman, char c, pMap map, pPowerup* powerups, int* numPowerups, pGhost ghosts, int numGhosts, pPortal* portals, int numPortals) {
    if(c == ' ')
        paused = 1 - paused;

    if(!paused) {
        if(pacman->CPU)
            pacmanChooseDirection(pacman, map, map->elems, powerups, *numPowerups, ghosts, numGhosts, &c);    // make pacman move by itself (CPU)
        char sprite = pacmanChangeDirection(pacman, c);
        pacmanMove(pacman, sprite, map, powerups, numPowerups, ghosts, numGhosts, portals, numPortals);
        ghostsMove(ghosts, numGhosts, pacman, map);
    }
}

// draws everything on the board
// the map is drawn every frame, which is of course not desirable
//   this was originally for debugging, but hasn't yet been changed back to only drawing things that changed since the last frame
void draw(pPacman pacman, pMap map, pPowerup* powerups, int numPowerups, pGhost ghosts, int numGhosts, pPortal* portals, int numPortals) {
    drawMap(map, map->elems);
    drawWalls(map, map->elems);
    powerupsDraw(powerups, numPowerups);
    portalsDraw(portals, numPortals);
    ghostsDraw(ghosts, numGhosts, map, map->elems);
    pacmanDraw(pacman);
}

// loads the scores into a binary search tree, inserts the current player's score prints them in order,
//   writes the new set of scores to the file, frees them, and frees the tree
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