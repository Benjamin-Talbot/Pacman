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