#ifndef MAP_H
#define MAP_H

typedef struct Map {
    int width;
    int height;
    char* elems;
} Map;
typedef Map* pMap;

#endif