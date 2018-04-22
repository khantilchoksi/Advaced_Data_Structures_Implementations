#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Invalid number of inputs.\nPlease add input in the form convert bin-file\n");
        exit(1);
    }

    char filename[100];
    strcpy(filename, argv[1]);

    FILE *binFile = fopen(filename, "rb");
    FILE *txtFile = fopen(strcat(filename, ".txt"), "w");

    int number;

    while(!feof(binFile)) {
        fread(&number, sizeof(int), 1, binFile);
        fprintf(txtFile, "%d\n", number);
    }
}