#include "score.h"

// initializes a node struct
pNode nodeInit(int score, char* name) {
    pNode node = (pNode) malloc(sizeof(Node));
    node->score = score;
    node->name = name;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// initializes a tree struct
pTree treeInit(pTree tree, int score, char* name) {
    pNode node = nodeInit(score, name);
    tree = (pTree) malloc(sizeof(Tree));
    tree->head = node;

    return tree;
}

// adds a node to a tree to make a binary search tree
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

// loads the scores from the file scores.txt
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
        numchars = 0;
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

// writes a single score to the file scores.txt
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

// writes the scores to the file scores.txt
void writeScores(pNode head) {
    if(head->left)
        writeScores(head->left);
    writeScore(head);
    if(head->right)
        writeScores(head->right);
}

// prints the scores
void printScores(pNode head) {
    if(head->right)
        printScores(head->right);
    printf("%d.\t%s: %d\n", rank, head->name, head->score);
    rank++;
    if(head->left)
        printScores(head->left);
}

// prints the binary search tree, for debugging purposes
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