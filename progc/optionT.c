#include "optionT.h"

City *MakeCity(char *name) {
    City *city = malloc(sizeof(City));
    city->name = name;
    city->count = 1;
    city->departureCount = 0;

    return city;
}

T_Node *MakeTNode(char *name) {
    T_Node *node = malloc(sizeof(T_Node));
    node->key = MakeCity(name);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

int T_Height(T_Node *N) {
    return N == NULL ? 0 : N->height;
}

int T_Max(int a, int b) {
    return a > b ? a : b;
}

T_Node *TRotateRight(T_Node *y) {
    if (y->left == NULL) return NULL;

    T_Node *x = y->left;
    T_Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = T_Max(T_Height(y->left), T_Height(y->right)) + 1;
    x->height = T_Max(T_Height(x->left), T_Height(x->right)) + 1;

    return x;
}

T_Node *TRotateLeft(T_Node *x) {
    if (x->right == NULL) return NULL;

    T_Node *y = x->right;
    T_Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = T_Max(T_Height(x->left), T_Height(x->right)) + 1;
    y->height = T_Max(T_Height(y->left), T_Height(y->right)) + 1;

    return y;
}

int TGetBalance(T_Node *N) {
    return N == NULL ? 0 : T_Height(N->left) - T_Height(N->right);
}

void AddValueIntList(IntLList *list, int val) {
    if (list == NULL) return;

    IntLList *next = list;
    while(next->next != NULL) {
        next = next->next;
    }

    IntLList *new = malloc(sizeof(IntLList));
    new->value = val;
    next->next = new;
}

int ValueInList(IntLList *list, int val) {
    IntLList *next = list;
    while(next != NULL) {
        if (next->value == val) return 1;
        next = next->next;
    }

    return 0;
}

T_Node *TInsertNode(T_Node *node, char *name, int departure, int routeId) {
    if (node == NULL) return (MakeTNode(name));

    int cmp = strcmp(node->key->name, name);
    if (cmp < 0)
        node->left = TInsertNode(node->left, name, departure, routeId);
    else if (cmp > 0)
        node->right = TInsertNode(node->right, name, departure, routeId);
    else {
        if (ValueInList(node->key->routeIds, routeId)) return node; // On compte une fois par trajet

        AddValueIntList(node->key->routeIds, routeId);
        if (departure) node->key->departureCount++;
        node->key->count++;
        return node;
    }

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + T_Max(T_Height(node->left),
                           T_Height(node->right));

    int balance = TGetBalance(node);

    int left_cmp = node->left == NULL ? 0 : strcmp(node->left->key->name, name);
    if (balance > 1 && left_cmp < 0)
        return TRotateRight(node);

    int right_cmp = node->right == NULL ? 0 : strcmp(node->right->key->name, name);
    if (balance < -1 && right_cmp > 0)
        return TRotateLeft(node);

    if (balance > 1 && left_cmp > 0) {
        node->left = TRotateLeft(node->left);
        return TRotateRight(node);
    }

    if (balance < -1 && right_cmp < 0) {
        node->right = TRotateRight(node->right);
        return TRotateLeft(node);
    }

    return node;
}

void TInsertNodeOrderedList(T_Node *node, T_Node **nodes) {
    // La liste est triée décroissante
    int i = 0;
    for (; i < 10; ++i) if (nodes[i] == NULL || node->key->count > nodes[i]->key->count) break;
    if (i == 10) return; // Aucune valeur à ajouter

    T_Node *previous = node;
    T_Node *tmp;
    for (; i < 10 && previous != NULL; ++i) {
        tmp = nodes[i];
        nodes[i] = previous;
        previous = tmp;
    }
}

void SearchTenHighestValues(T_Node *root, T_Node **nodes) {
    if (root == NULL) return;

    TInsertNodeOrderedList(root, nodes);

    SearchTenHighestValues(root->left, nodes);
    SearchTenHighestValues(root->right, nodes);
}


void TOutputHighestValues(T_Node *root, char *outputFilePath) {
    if (root == NULL) return;

    FILE *output = fopen(outputFilePath, "w");
    if (output == NULL)
    {
        printf("Error opening the output file");
        return;
    }

    T_Node **nodes = malloc(10 * sizeof(T_Node*));
    SearchTenHighestValues(root, nodes);
    for (int i = 0; i < 10; ++i) {
        fprintf(output, "%d;%s;%d;%d\n", i + 1, nodes[i]->key->name, nodes[i]->key->count, nodes[i]->key->departureCount);
    }

    fclose(output);
}

void T(FILE *file, char* outputFilePath) {
    T_Node *root = NULL;

    char buffer[1024];
    int line_number = 1;
    fgets(buffer, 1024, file);
    while (fgets(buffer, 1024, file)) {
        buffer[strcspn(buffer, "\n")] = 0; // On enlève les \n en fin chaine

        char *subString = strtok(buffer, ";");
        int i = 0;
        int id = -1;
        while(subString != NULL)
        {
            if (i == 0) {
                id = atoi(subString);
            }
            if (i == 2) {
                char *name = malloc(32);
                strcpy(name, subString);
                root = TInsertNode(root, name, 1, id);
            }
            else if (i == 3) {
                char *name = malloc(32);
                strcpy(name, subString);
                root = TInsertNode(root, name, 0, id);
            }

            i++;
            subString = strtok(NULL, ";");
        }
        line_number++;
    }

    TOutputHighestValues(root, outputFilePath);
}