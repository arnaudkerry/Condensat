#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "algorithms.h"
#include "filehelper.h"
/**
 * @brief Affichage de l'aide d'utilisation
 * 
 */
void displayHelp() {
    printf("Usage: ./main OPTION ... [FILE]...\n\n-g, --generate\tgenerate mode.\n-l, --lookup\tlookup mode.\n\n-g FILEIN [-out FILEOUT]:\nGenerate mode : Show the hash of the entries.\n-out FILEOUT : Stores the results into a file.\n-algo ALGO : The algorithm used to encrypt the words (default SHA256) [SHA256,SHA1,SHA512]\n\n -l FILEIN :\nLookup mode : Searching a plainword from a cipher (FILEIN will be used as the database)\n");
}


/**
 * @brief Mode interactif => permet de boucler sur plusieurs recherches
 * 
 * @param root 
 */
void interactiveMode(HashStruct* root) {
    printf("<<<< Interactive mode >>>> (q for quit)\n");
    printf("Search plain for : ");
    unsigned char* choice = malloc(sizeof(unsigned char) * 200);
    fgets(choice,200,stdin);
    if(strcmp(choice,"q\n") == 0) {
        exit(0);
        free(choice);
        destroy_hash_lookup(root);
    } else {
        search(choice,root,0);
    }
}

/**
 * @brief Mode lookup
 * 
 * @param parser 
 */
void lookup(Parser * parser) {
    HashStruct * root = NULL;
    fileLoad(parser->fileIn,&root);
    printf("Chargement terminé\n");
    while(1) {
        printf("wsh");
        interactiveMode(root);
    }
}


/**
 * @brief Lancement du programme selon les options récupérées par le Parser
 * 
 * @param parser 
 */
void launch(Parser * parser) {
    if(parser->isGenerate == 0) {
        fileEncrypt(parser);
    } else if (parser->isLookup == 0) {
        lookup(parser);
    } else {
        displayHelp();
        exit(0);
    }
}

/**
 * @brief Récupération et parsing des arguments en ligne de commande
 * 
 * @param argc 
 * @param argv 
 * @return Parser* 
 */
Parser* parseArgs(int argc, char* argv[]) {
    int i = 0;
    Parser * parser = parser_constructor();
    while(argv[i] != NULL) {
    if(strcmp(argv[i],"-g") == 0 || strcmp(argv[i],"-l") == 0) 
    {
        if(strcmp(argv[i],"-g") == 0) {
            parser->isGenerate = 0;
        } else if(strcmp(argv[i],"-l") == 0) {
            parser->isLookup = 0;
        } else {
            displayHelp();
            exit(0);
        }
        if(argv[i+1] != NULL) {
            parser->fileIn = argv[++i];
        } else {
            displayHelp();
            exit(1);
        }
    } else if (strcmp(argv[i],"-out") == 0)
    {
        parser->isOut = 0;
        if(argv[i+1] != NULL) {
            parser->fileOut = argv[++i];
        } else {
            displayHelp();
            exit(1);
        }
    } else if (strcmp(argv[i],"-algo") == 0)
    {
        parser->isAlgorithm = 0;
        if(argv[i+1] != NULL) {
            parser->algorithm = argv[++i];
        } else {
            displayHelp();
            exit(1);
        }
    }
    i++;
    }
    return parser;
}

/***
 * Fonction main
*/
int main(int argc, char *argv[]) {
    Parser *parser = parseArgs(argc,argv);
    launch(parser);
    destroy_parser(parser);
}


