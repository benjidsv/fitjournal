#ifndef PROJETG_OPTIONT_H
#define PROJETG_OPTIONT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct IntLList {
    int value;
    struct IntLList *next;
} IntLList;

typedef struct City {
    char *name;
    int count;
    int departureCount;
    IntLList *routeIds;
} City;

typedef struct T_Node {
    City *key;
    struct T_Node *left;
    struct T_Node *right;
    int height;
} T_Node;

void T(FILE *file, char* outputFilePath);

#endif //PROJETG_OPTIONT_H
