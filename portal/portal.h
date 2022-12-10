#ifndef PORTAL_H
#define PORTAL_H

typedef struct Portal {
    int x;
    int y;
    char sprites[2];
    int sprite;
    struct Portal* exit;
} Portal;
typedef Portal* pPortal;

#endif