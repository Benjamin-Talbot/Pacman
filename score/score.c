#include "score.h"

pNode nodeInit(int score, char* name) {
    pNode node = (pNode) malloc(sizeof(Node));
    node->score = score;
    node->name = name;

    return node;
}

pTree treeInit(pTree tree, int score, char* name) {
    tree = (pTree) malloc(sizeof(Tree));
    pNode node = nodeInit(score, name);
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
    // scores = (pTree) malloc(sizeof(Tree));
    scores = NULL;

    FILE* scoresFile = fopen("score/scores.txt", "r");
    int success = TRUE;
    char c = ' ';
    int lenName = 8;
    char* name = (char*) malloc(sizeof(char) * lenName);
    int numchars = 0;
    int score = 0;

    while(success > 0) {
        while(c != ':' && success > 0){
            success = fscanf(scoresFile, "%c", &c);
            if(success > 0 && c != ':') {
                numchars++;
                if(numchars >= lenName / sizeof(char)) {
                    lenName *= 2;
                    name = realloc(name, lenName);
                }
                strncat(name, &c, 1);
            }
        }
        char* tmp = (char*) malloc(sizeof(char) * numchars);
        strncpy(tmp, name, numchars);
        fscanf(scoresFile, "%d", &score);
        if(success > 0) {
            if(scores) {
                addNode(scores->head, score, tmp);
            }
            else {
                scores = (pTree) malloc(sizeof(Tree));
                scores = treeInit(scores, score, tmp);
            }
        }
        fscanf(scoresFile, "%c", &c);
        if(c == '\n') {
            free(name);
            lenName = 8;
            name = (char*) malloc(sizeof(char) * lenName);
        }
        else
            success = FALSE;
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
    
    if(numWritten != rank - 1)
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
    printf("%d. %s: %d\n", rank, head->name, head->score);
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

/**
int main() {
    pTree tree = treeInit(tree, 0, "Ben");
    addNode(tree->head, 1, "Carl");
    addNode(tree->head, 2, "A");
    addNode(tree->head, 3, "B");
    addNode(tree->head, 1, "C");

    printTree(tree->head, 0);

    return 0;
}
//*/