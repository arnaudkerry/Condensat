#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"

#define MAX_LENGTH 1000

void displayHelp() {
    printf("I need options to work\n");
}

void md5encrypt() {
    printf("Chiffrage en MD5\n");
}

void sha256encrypt() {
    printf("Chiffrage en sha256\n");
}

char* encrypt(char* word, char* algorithm) {
    return "encrypted";
}

void fileEncrypt(char* filename, char* algorithm) {
    FILE* file = NULL;
    char line[MAX_LENGTH] = "";
    file = fopen(filename,"r+");
    if(file != NULL) {
        while(fgets(line, MAX_LENGTH, file) != NULL) {
            char *ptr = strchr(line, '\n');
            if(ptr)
                {
                    *ptr = '\0';
                }
            char* encrypted = encrypt(line,algorithm);
            printf("%s\t%s\n",line,encrypted);
        }
        fclose(file);
    } else {
        printf("Impossible d'ouvrir le fichier\n");
    }
}

char* chooseAlgorithm(int argc, char* argv) {
    char* algorithm = NULL;
    if(argc >= 4) {
        algorithm = argv[3];
    }

    if(algorithm == NULL) {
        algorithm = "sha256";
    }

    return algorithm;
}

void generateMode(int argc, char* argv[]) {
    char* filename = argv[2];
    char* algorithm = chooseAlgorithm(argc,argv);
    char* fileOut = NULL;
    //TODO : Faire l'option de sortie
    fileEncrypt(filename,algorithm);
}

void listMode(int argc, char* argv) {

}


void parseArgs(int argc, char* argv[]) {
    //argc = nombre d'args (le premier étant le nom du fichier exécutable)
    char * algorithm = NULL;
    if(argc == 1) {
        displayHelp();
    } else {
        if(strcmp(argv[1],"g") == 0 || strcmp(argv[1],"G") == 0 || strcmp(argv[1],"generate") == 0) {
            generateMode(argc,argv);
        } else if (strcmp(argv[1],"l") == 0 || strcmp(argv[1],"L") == 0 || strcmp(argv[1],"list") == 0) {
            listMode(argc,argv);
        } else {
            displayHelp();
        }
    }
}

int main(int argc, char *argv[]) {
    parseArgs(argc,argv);
}


