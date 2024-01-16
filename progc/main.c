#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct City {
    char *name;
    int count;
    int departureCount;
} City;

City *MakeCity(char *name) {
    City *city = malloc(sizeof(City));
    city->name = name;
    city->count = 1;
    city->departureCount = 0;

    return city;
}

typedef struct T_Node {
    City *key;
    struct T_Node *left;
    struct T_Node *right;
    int height;
} T_Node;

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

int T_Height(T_Node *N) {
    return N == NULL ? 0 : N->height;
}

int S_Height(S_Node *N) {
    return N == NULL ? 0 : N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

T_Node *MakeTNode(char *name) {
    T_Node *node = malloc(sizeof(T_Node));
    node->key = MakeCity(name);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

S_Node *MakeSNode(int id) {
    S_Node *node = malloc(sizeof(S_Node));
    node->routeID = id;
    node->distances = malloc(sizeof(LList));
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

T_Node *TRotateRight(T_Node *y) {
    if (y->left == NULL) return NULL;

    T_Node *x = y->left;
    T_Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(T_Height(y->left), T_Height(y->right)) + 1;
    x->height = max(T_Height(x->left), T_Height(x->right)) + 1;

    return x;
}

T_Node *TRotateLeft(T_Node *x) {
    if (x->right == NULL) return NULL;

    T_Node *y = x->right;
    T_Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(T_Height(x->left), T_Height(x->right)) + 1;
    y->height = max(T_Height(y->left), T_Height(y->right)) + 1;

    return y;
}

S_Node *SRotateRight(S_Node *y) {
    if (y->left == NULL) return NULL;

    S_Node *x = y->left;
    S_Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(S_Height(y->left), S_Height(y->right)) + 1;
    x->height = max(S_Height(x->left), S_Height(x->right)) + 1;

    return x;
}

S_Node *SRotateLeft(S_Node *x) {
    if (x->right == NULL) return NULL;

    S_Node *y = x->right;
    S_Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(S_Height(x->left), S_Height(x->right)) + 1;
    y->height = max(S_Height(y->left), S_Height(y->right)) + 1;

    return y;
}

int TGetBalance(T_Node *N) {
    return N == NULL ? 0 : T_Height(N->left) - T_Height(N->right);
}

int SGetBalance(S_Node *N) {
    return N == NULL ? 0 : T_Height(N->left) - T_Height(N->right);
}

T_Node *TInsertNode(T_Node *node, char *name, int departure) {
    if (node == NULL) return (MakeTNode(name));

    int cmp = strcmp(node->key->name, name);
    if (cmp < 0)
        node->left = TInsertNode(node->left, name, departure);
    else if (cmp > 0)
        node->right = TInsertNode(node->right, name, departure);
    else {
        if (departure) node->key->departureCount++;
        node->key->count++;
        return node;
    }

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + max(T_Height(node->left),
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

void AddToLList(LList *list, double dst) {
    if (list->value == 0) {
        list->value = dst;
        return;
    }

    LList *next = list;
    while (next->next != NULL) {
        next = next->next;
    }

    LList *new = malloc(sizeof(LList));
    new->value = dst;

    next->next = new;
}

S_Node *SInsertNode(S_Node *node, int id, double dst) {
    if (node == NULL) return (MakeSNode(id));

    if (id < node->routeID)
        node->left = SInsertNode(node->left, id, dst);
    else if (id > node->routeID)
        node->right = SInsertNode(node->right, id, dst);
    else {
        AddToLList(node->distances, dst);
        return node;
    }

    // Update the balance factor of each node and
    // Balance the tree
    node->height = 1 + max(S_Height(node->left),
                           S_Height(node->right));

    int balance = SGetBalance(node);

    if (balance > 1 && id < node->left->routeID)
        return SRotateRight(node);

    if (balance < -1 && id > node->right->routeID)
        return SRotateLeft(node);

    if (balance > 1 && id > node->left->routeID) {
        node->left = SRotateLeft(node->left);
        return SRotateRight(node);
    }

    if (balance < -1 && id > node->right->routeID) {
        node->right = SRotateRight(node->right);
        return SRotateLeft(node);
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

struct LListInfo {
    double min;
    double max;
    double average;
};

struct LListInfo GetLListInfo(LList *list) {
    struct LListInfo info;
    info.min = list->value;
    info.max = list->value;

    int count = 0;
    double sum = 0;
    LList *next = list;
    while (next != NULL) {
        if (next->value < info.min) info.min = next->value;
        else if (next->value > info.max) info.max = next->value;

        sum += next->value;
        count++;
        next = next->next;
    }

    info.average = sum/count;
    return info;
}

void SInsertNodeOrderedList(S_Node *node, S_Node **nodes) {
    // La liste est triée décroissante
    int i = 0;
    for (; i < 50; ++i) {
        if (nodes[i] == NULL) break;
        struct LListInfo info = GetLListInfo(node->distances);
        struct LListInfo info2 = GetLListInfo(nodes[i]->distances);
        if (info.max - info.min > info2.max - info.min) break;
    }
    if (i == 50) return; // Aucune valeur à ajouter

    S_Node *previous = node;
    S_Node *tmp;
    for (; i < 50 && previous != NULL; ++i) {
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

void TPrintHighestValues(T_Node *root) {
    if (root == NULL) return;

    T_Node **nodes = malloc(10 * sizeof(T_Node*));
    SearchTenHighestValues(root, nodes);
    for (int i = 0; i < 10; ++i) {
        printf("%d | %s - %d - %d\n", i + 1, nodes[i]->key->name, nodes[i]->key->count, nodes[i]->key->departureCount);
    }
}

void SearchFiftyHighestValues(S_Node *root, S_Node **nodes) {
    if (root == NULL) return;

    SInsertNodeOrderedList(root, nodes);

    SearchFiftyHighestValues(root->left, nodes);
    SearchFiftyHighestValues(root->right, nodes);
}

void SPrintHighestValues(S_Node *root) {
    if (root == NULL) return;

    S_Node **nodes = malloc(50 * sizeof(S_Node*));
    SearchFiftyHighestValues(root, nodes);
    for (int i = 0; i < 50; ++i) {
        struct LListInfo info = GetLListInfo(nodes[i]->distances);
        printf("%d | %d - %f - %f - %f\n", i + 1, nodes[i]->routeID, info.min, info.average, info.max);
    }
}

T_Node **GetTenHighestCountCities(T_Node *root) {
    T_Node **nodes = malloc(10 * sizeof(T_Node*));
    SearchTenHighestValues(root, nodes);
    return nodes;
}

void PrintPreOrder(T_Node *root) {
    if (root != NULL) {
        printf("%s : %d\n", root->key->name, root->key->count);
        PrintPreOrder(root->left);
        PrintPreOrder(root->right);
    }
}

T_Node *T(FILE *file) {
    T_Node *root = NULL;

    char buffer[1024];
    int line_number = 1;
    fgets(buffer, 1024, file);
    while (fgets(buffer, 1024, file)) {
        buffer[strcspn(buffer, "\n")] = 0; // On enlève les \n en fin chaine

        char *subString = strtok(buffer, ";");
        int i = 0;
        while(subString != NULL)
        {
            if (i == 2) {
                char *name = malloc(32);
                strcpy(name, subString);
                root = TInsertNode(root, name, 1);
            }
            else if (i == 3) {
                char *name = malloc(32);
                strcpy(name, subString);
                root = TInsertNode(root, name, 0);
            }

            i++;
            subString = strtok(NULL, ";");
        }
        line_number++;
    }

    TPrintHighestValues(root);
}

S_Node *S(FILE *file) {
    S_Node *root = NULL;

    char buffer[1024];
    int line_number = 1;
    fgets(buffer, 1024, file);
    while (fgets(buffer, 1024, file)) {
        buffer[strcspn(buffer, "\n")] = 0; // On enlève les \n en fin chaine

        char *subString = strtok(buffer, ";");
        int i = 0;
        int id = -1;
        double dst = -1;
        while(subString != NULL)
        {
            if (i == 0) {
                id = atoi(subString);
            }
            else if (i == 4) {
                char *_;
                dst = strtod(subString, &_);
            }
            else if (i == 5) {
                root = SInsertNode(root, id, dst);
            }

            i++;
            subString = strtok(NULL, ";");
        }
        line_number++;
    }

    SPrintHighestValues(root);
}

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");//replace by arg

    if (!file) {
        printf("Impossible d'ouvrir le fichier\n");
        return 1;
    }

    char arg = *argv[2];
    if (arg == 'T') T(file);
    else if (arg == 'S') S(file);
    else abort();

    fclose(file);

    return 0;
}