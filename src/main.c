#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "algorithms.h"

#define MAX_LENGTH 1000

void displayHelp() {
    printf("Usage: ./main OPTION ... [FILE]...\n\n-g, --generate\tgenerate mode.\n-l, --lookup\tlookup mode.\n\n-g FILEIN [-out FILEOUT]:\nGenerate mode : Show the hash of the entries.\n-out FILEOUT : Stores the results into a file.\n-algo ALGO : The algorithm used to encrypt the words (default SHA256) [SHA256,SHA1,SHA512]\n\n -l FILEIN :\nLookup mode : Searching a plainword from a cipher (FILEIN will be used as the database)\n");
}


HashStruct * encrypt(unsigned char* word, unsigned char* algorithm) {
    if(strcmp(algorithm,"SHA512") == 0) {
        return sha512cryption(word);
    } else if (strcmp(algorithm,"SHA1") == 0)
    {
        return sha1cryption(word);
    } else if (strcmp(algorithm,"SHA256") == 0)
    {
        return sha256cryption(word);
    } else {
        printf("Algorithm not find... cryption with SHA256...");
        return sha256cryption(word);
    }
}

void fileEncrypt(Parser* parser) {
    FILE* file = NULL;
    FILE* fileout = NULL;
    if(parser->fileOut)    
        fileout = fopen(parser->fileOut,"w+");
    unsigned char* line = malloc(sizeof(unsigned char) * MAX_LENGTH);
    file = fopen(parser->fileIn,"r+");
    if(file != NULL) {
        while(fgets(line, MAX_LENGTH, file) != NULL) {
            unsigned char *ptr = strchr(line, '\n');
            if(ptr)
                {
                    *ptr = '\0';
                }
            HashStruct* encrypted = encrypt(line,parser->algorithm);
            if(fileout != NULL) {
                fprintf(fileout,"%s\t%s\n",line,encrypted->buffered);
            } else {
                printf("%s\t%s\n",line,encrypted->buffered);
            }
            destroy_hash_generate(encrypted);
        }
        fclose(file);
        if(fileout)
            fclose(fileout);
        free(line);
    } else {
        printf("Impossible d'ouvrir le fichier\n");
    }
}

void fileLoad(unsigned char* filename,HashStruct** root,int k) {
    FILE* file = NULL;
    unsigned char* word = NULL;
    unsigned char* hash = NULL;
    unsigned char line[MAX_LENGTH] = "";
    file = fopen(filename,"r+");
    if(file != NULL) {
        while(fgets(line,MAX_LENGTH,file) != NULL) {
            
            unsigned char *ptr = strchr(line,'\n');
            if(ptr)
            {
                *ptr = '\0';
            }
            for(int i = 0; i<= strlen(line); i++) {
                if(line[i] == '\t') {
                    line[i] = '\0';
                    word = line;
                    hash = line+1+i;
                    break;
                }
            }
            add(root,word,hash,0,k++);
        }
    }
    fclose(file);
}

void searchRecurs(unsigned char* hash,HashStruct* root, int count) {
    if(strcmp(hash,root->buffered) == 0) {
        printf("The plainword for %s is %s\n",hash,root->plainWord);
    } else if (hash[count] > root->buffered[count] && root->left)
            searchRecurs(hash,root->left,0);
        else if (hash[count] < root->buffered[count] && root->right)
            searchRecurs(hash,root->right,0);
        else if(hash[count] == root->buffered[count]) {
            count++;
            searchRecurs(hash,root,count);
        }
}

void search(unsigned char* hash,HashStruct* root,int count) {
    hash[strcspn(hash,"\n")] = 0;
    if(strcmp(hash,root->buffered) == 0) {
        printf("The plainword for %s is %s\n",hash,root->plainWord);
    } else {
        if(hash[count] > root->buffered[count] && root->left) {
            searchRecurs(hash,root->left,0);
        } else if (hash[count] < root->buffered[count] && root->right) {
            searchRecurs(hash,root->right,0);
        } else if(hash[count] == root->buffered[count]) {
            count++;
            searchRecurs(hash,root,count);
        }
    }
}

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


void lookup(Parser * parser) {
    HashStruct * root = NULL;
    int k = 0;
    fileLoad(parser->fileIn,&root,k);
    printf("Chargement terminé\n");
    while(1) {
        printf("wsh");
        interactiveMode(root);
    }
}



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

int main(int argc, char *argv[]) {
    Parser *parser = parseArgs(argc,argv);
    launch(parser);
    destroy_parser(parser);
}


