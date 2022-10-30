#include "classes.h"

HashStruct * sha256cryption(unsigned char* input) {
    unsigned char * output = malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH);
    HashStruct * hash = hashstruct_constructor_generate(SHA256(input,strlen(input),output),SHA256_DIGEST_LENGTH,input);
    free(output);
    return hash;
}

HashStruct * sha1cryption(unsigned char* input) {
    unsigned char * output = malloc(sizeof(unsigned char) * SHA_DIGEST_LENGTH);
    HashStruct * hash = hashstruct_constructor_generate(SHA1(input,strlen(input),output),SHA_DIGEST_LENGTH,input);
    free(output);
    return hash;
}

HashStruct * sha512cryption(unsigned char* input) {
    unsigned char * output = malloc(sizeof(unsigned char) * SHA512_DIGEST_LENGTH);
    HashStruct * hash = hashstruct_constructor_generate(SHA512(input,strlen(input),output),SHA512_DIGEST_LENGTH,input);
    free(output);
    return hash;
}

/**
 * @brief Fonction récursive de recherche
 * 
 * @param hash 
 * @param root 
 * @param count 
 */
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

/**
 * @brief Recherche d'un mot en clair selon un condensat
 * Appel de la fonction récursive
 * 
 * @param hash 
 * @param root 
 * @param count 
 */
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

/**
 * @brief Fonction récursive d'ajout dans l'arbre binaire
 * 
 * @param current 
 * @param toAdd 
 * @param count 
 */
void addRecurs(HashStruct* current, HashStruct* toAdd,int count) {
    if(strcmp(current->plainWord,toAdd->plainWord) == 0) {
        return;
    }
    if(current->buffered[count] > toAdd->buffered[count]) {
    if(!current->right) {
        current->right = toAdd;
        return;
    }
    else {
        current = current->right;
        addRecurs(current,toAdd,0);
        return;
    }
} else if (current->buffered[count] < toAdd->buffered[count]) {
    if(!current->left) {
        current->left = toAdd;
        return;
    }
    else {
        current = current->left;
        addRecurs(current,toAdd,0);
        return;
    }
    } else if(current->buffered[count] == toAdd->buffered[count]) {
        addRecurs(current,toAdd,++count);
        return;
    }
}

/**
 * @brief Fonction d'ajout dans l'arbre binaire
 * Appel de la fonction récursive
 * @param root 
 * @param word 
 * @param hash 
 * @param count 
 * @param k 
 */
void add(HashStruct** root,unsigned char* word,unsigned char* hash,int count) {
    HashStruct* newHashStruct = hashstruct_constructor_lookup(word,hash);
    if(!*root) {
        *root = newHashStruct;
        return;
    }

    HashStruct* current = *root;
    if(strcmp(current->plainWord,newHashStruct->plainWord) == 0) {
        return;
    }
    if(current->buffered[count] > newHashStruct->buffered[count]) {
        if(!current->right) {
            current->right = newHashStruct;
            return;
        }
        else {
            current = current->right;
            addRecurs(current,newHashStruct,0);
            return;
        }
    } else if (current->buffered[count] < newHashStruct->buffered[count]) {
        if(!current->left) {
            current->left = newHashStruct;
            return;
        }
        else {
            current = current->left;
            addRecurs(current,newHashStruct,0);
            return;
        }
    } else if (current->buffered[count] == newHashStruct->buffered[count]) {
        addRecurs(current,newHashStruct,++count);
        return;
    }
}

/**
 * @brief Fonction qui va chiffrer le mot en entrée selon l'algorithme choisi
 * 
 * @param word 
 * @param algorithm 
 * @return HashStruct* 
 */
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
