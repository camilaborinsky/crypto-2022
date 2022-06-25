#ifndef _CRYPTOGRAPHY_H_
#define _CRYPTOGRAPHY_H_
#define ENCRYPTION_ALGORITHMS_NUMBER 4
#define BLOCK_CHAINING_TYPE_NUMBER 4
#define MAX_CIPHER_NAME_SIZE 13
#include <structs.h>
#include <openssl/evp.h>


enum crypto_algorithm {
    AES_128,
    AES_192,
    AES_256,
    DES
};

enum crypto_block_algorithm{
    ECB,
    CFB,
    OFB,
    CBC
};


static int encrypt_decrypt(
    const uint8_t* in, 
    const uint32_t in_size, 
    const uint8_t* password, 
    enum crypto_algorithm algorithm, 
    enum crypto_block_algorithm block_chaining_type,
    uint8_t* out,
    uint32_t* out_size,
    int mode
);

static const EVP_CIPHER* get_cipher(
    enum crypto_algorithm algorithm,
    enum crypto_block_algorithm block_chaining_type
);

int decrypt(FILE* encrypted_file, Parameters params, char * decrypted_data);

enum crypto_algorithm get_encryption_algorithm(char * algorithm);
enum crypto_block_algorithm get_block_algorithm(char * algorithm);

#endif