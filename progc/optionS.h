#ifndef PROJETG_OPTIONS_H
#define PROJETG_OPTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct LList {
    double value;
    struct LList *next;
} LList;

typedef struct S_Node {
    int routeID;
    LList *distances;

    struct S_Node *left;
    struct S_Node *right;
    int height;
} S_Node;

void S(FILE *file, char* outputFilePath);

#endif //PROJETG_OPTIONS_H
