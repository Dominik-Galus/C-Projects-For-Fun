#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
     FILE* fp = fopen(argv[1], "w");

     if (fp == NULL) {
         perror("Error with file handling");
         exit(-1);
    }

    int c;

    while ((c = getchar()) != EOF) {
        putchar(c);
        putc(c, fp);
    }

    return 0;
}
