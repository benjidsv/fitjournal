#include "optionS.h"
#include "optionT.h"

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");

    if (argc < 3) return -1;

    if (!file) {
        printf("Impossible d'ouvrir le fichier\n");
        return 1;
    }

    char arg = *argv[2];
    if (arg == 'T') T(file, "output.txt");
    else if (arg == 'S') S(file, "output.txt");
    else abort();

    fclose(file);

    return 0;
}
