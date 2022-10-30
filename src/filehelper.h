
#define MAX_LENGTH 1000

/**
 * @brief Parcours du fichier, chiffrement des entrées et affichage (ou sauvegarde) des résultats
 * 
 * @param parser 
 */
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

/**
 * @brief Création de l'arbre binaire de recherche à partir du fichier d'entrée
 * 
 * @param filename 
 * @param root 
 */
void fileLoad(unsigned char* filename,HashStruct** root) {
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
            add(root,word,hash,0);
        }
    }
    fclose(file);
}