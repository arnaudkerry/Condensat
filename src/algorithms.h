#include "classes.h"
// /!\ Lorsque on utilise ces fonctions, le buffered correspond au hash
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
