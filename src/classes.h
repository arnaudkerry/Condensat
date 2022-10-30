/**
 * @brief Retranscription des hash en un format facilement imprimable et compréhensible
 * 
 * @param hash 
 * @param length 
 * @return unsigned* 
 */
unsigned char* bufHash(unsigned char * hash,int length) {
    unsigned char* buf = malloc(sizeof(unsigned char*) * 2 * length);
    memset(buf, 0x0, length*2);
    
    for(int i = 0; i < length; i++) {
        sprintf((unsigned char*)&(buf[i*2]), "%02x", hash[i]);
    }
    
    return buf;
}

/**
 * @brief Classe HASHSTRUCT, qui va former les éléments d'un arbre binaire de recherche
 * 
 */
struct hashStruct {
    int length;
    unsigned char* hash;
    unsigned char* buffered;
    unsigned char* plainWord;
    struct hashStruct* left;
    struct hashStruct* right;
};
typedef struct hashStruct HashStruct;

/**
 * @brief Fonction de destruction des HASHSTRUCT lors du mode generate
 * 
 * @param h 
 */
void destroy_hash_generate(HashStruct * h) {
    free(h->buffered);
    free(h);
}

/**
 * @brief Fonction de destruction des HASHSTRUCT créés lors du mode lookup
 * 
 * @param h 
 */
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

/**
 * @brief Constructeur des HASHSTRUCT en mode generate
 * 
 * @param hash 
 * @param length 
 * @param input 
 * @return HashStruct* 
 */
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

/**
 * @brief Constructeur des HASHSTRUCT en mode lookup
 * 
 * @param plain 
 * @param hash 
 * @return HashStruct* 
 */
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


/**
 * @brief Class PARSER
 * 
 */
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

/**
 * @brief Fonction de destruction de la class PARSER
 * 
 * @param p 
 */
void destroy_parser(Parser* p) {
    free(p);
}

/**
 * @brief Constructeur de la class PARSER
 * 
 * @return Parser* 
 */
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