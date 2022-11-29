#ifndef SCORE_H
#define SCORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

pNode nodeInit(int score, char* name);

pTree treeInit(pTree tree, int score, char* name);

void addNode(pNode head, int score, char* name);

pTree loadScores(pTree scores);

void writeScores(pNode head);

void printScores(pNode head);

void printTree(pNode head, int level);

#endif