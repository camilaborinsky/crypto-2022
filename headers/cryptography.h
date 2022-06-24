#ifndef _CRYPTOGRAPHY_H_
#define _CRYPTOGRAPHY_H_
#define ENCRYPTION_ALGORITHMS_NUMBER 4
#define ENCRYPTION_ALGORITHMS_NUMBER 4
#define MAX_CIPHER_NAME_SIZE 13

enum crypto_algorithm {
    AES_128,
    AES_192,
    AES_256,
    DES
};

enum crypto_block_algorithm{
    ECB,
    OFB,
    CBC
};


const char* cryptographic_algorithm_names[ENCRYPTION_ALGORITHMS_NUMBER] = {"aes-128-", "aes-192-", "aes-256-", "des-"};
const int cryptographic_algorithm_name_size[ENCRYPTION_ALGORITHMS_NUMBER] = {9, 9, 9, 5};

const char* block_chaining_type_names[BLOCK_CHAINING_TYPE_NUMBER] = {"ecb", "cfb", "ofb", "cbc"};
const int block_chaining_type_name_size[ENCRYPTION_ALGORITHMS_NUMBER] = {3, 3, 3, 3};

#endif