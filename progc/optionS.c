#include "optionS.h"

int S_Height(S_Node *N) {
    return N == NULL ? 0 : N->height;
}

int S_Max(int a, int b) {
    return a > b ? a : b;
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



S_Node *SRotateRight(S_Node *y) {
    if (y->left == NULL) return NULL;

    S_Node *x = y->left;
    S_Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = S_Max(S_Height(y->left), S_Height(y->right)) + 1;
    x->height = S_Max(S_Height(x->left), S_Height(x->right)) + 1;

    return x;
}

S_Node *SRotateLeft(S_Node *x) {
    if (x->right == NULL) return NULL;

    S_Node *y = x->right;
    S_Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = S_Max(S_Height(x->left), S_Height(x->right)) + 1;
    y->height = S_Max(S_Height(y->left), S_Height(y->right)) + 1;

    return y;
}



int SGetBalance(S_Node *N) {
    return N == NULL ? 0 : S_Height(N->left) - S_Height(N->right);
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
    node->height = 1 + S_Max(S_Height(node->left),
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
    struct LListInfo info = GetLListInfo(node->distances);
    for (; i < 50; ++i) {
        if (nodes[i] == NULL) break;
        struct LListInfo info2 = GetLListInfo(nodes[i]->distances);
        if (info.max - info.min > info2.max - info2.min) break;
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

void SearchFiftyHighestValues(S_Node *root, S_Node **nodes) {
    if (root == NULL) return;

    SInsertNodeOrderedList(root, nodes);

    SearchFiftyHighestValues(root->left, nodes);
    SearchFiftyHighestValues(root->right, nodes);
}

void SOutputHighestValues(S_Node *root, char* outputFilePath) {
    if (root == NULL) return;

    FILE *output = fopen(outputFilePath, "w");
    if (output == NULL)
    {
        printf("Error opening the output file");
        return;
    }

    S_Node **nodes = malloc(50 * sizeof(S_Node*));
    SearchFiftyHighestValues(root, nodes);
    for (int i = 0; i < 50; ++i) {
        struct LListInfo info = GetLListInfo(nodes[i]->distances);
        fprintf(output, "%d;%d;%f;%f;%f\n", i + 1, nodes[i]->routeID, info.min, info.average, info.max);
    }

    fclose(output);
}

void S(FILE *file, char* outputFilePath){
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
        while(subString != NULL) {
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

    SOutputHighestValues(root, outputFilePath);
}