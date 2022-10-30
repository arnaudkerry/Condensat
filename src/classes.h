unsigned char* bufHash(unsigned char * hash,int length) {
    unsigned char* buf = malloc(sizeof(unsigned char*) * 2 * length);
    memset(buf, 0x0, length*2);
    
    for(int i = 0; i < length; i++) {
        sprintf((unsigned char*)&(buf[i*2]), "%02x", hash[i]);
    }
    
    return buf;
}

struct hashStruct {
    int length;
    unsigned char* hash;
    unsigned char* buffered;
    unsigned char* plainWord;
    struct hashStruct* left;
    struct hashStruct* right;
};
typedef struct hashStruct HashStruct;

void destroy_hash_generate(HashStruct * h) {
    free(h->buffered);
    free(h);
}

void destroy_hash_lookup(HashStruct * h) {
    free(h->hash);
    free(h->buffered);
    free(h->plainWord);
    if(h->left)
        destroy_hash_lookup(h->left);
    if(h->right)
        destroy_hash_lookup(h->right);
    free(h);
}

HashStruct * hashstruct_constructor_generate(unsigned char* hash,int length,unsigned char* input) {
    HashStruct *h = malloc(sizeof(HashStruct));
    h->length = length;
    h->hash = hash;
    h->buffered = bufHash(hash,length);
    h->plainWord = input;
    h->left = NULL;
    h->right = NULL;
    return h;
}

HashStruct * hashstruct_constructor_lookup(unsigned char* plain,unsigned char* hash) {
    HashStruct *h = malloc(sizeof(HashStruct));
    h->length = strlen(hash);
    h->hash = malloc(strlen(hash) * sizeof(unsigned char)+1);
    h->buffered = malloc(strlen(hash) * sizeof(unsigned char)+1);
    h->plainWord = malloc(strlen(plain) * sizeof(unsigned char)+1);
    h->left = NULL;
    h->right = NULL;
    strcpy(h->hash,hash);
    strcpy(h->buffered,hash);
    strcpy(h->plainWord,plain);
    return h;
}

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

void add(HashStruct** root,unsigned char* word,unsigned char* hash,int count,int k ) {
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




typedef struct parser {
    //0 = true, 1 = False
    int isGenerate;
    char* fileIn;
    int isLookup;
    int isOut;
    char* fileOut;
    int isAlgorithm;
    char* algorithm;
} Parser;

void destroy_parser(Parser* p) {
    free(p);
}

Parser * parser_constructor() {
    Parser *p = malloc(sizeof(Parser));
    p->isAlgorithm = 1;
    p->algorithm = "SHA256";
    p->isGenerate = 1;
    p->fileIn = NULL;
    p->isLookup = 1;
    p->isOut = 1;
    p->fileOut = NULL;
}