#include <openssl/evp.h>
#include <cryptography.h>
#include <string.h>



static const EVP_CIPHER* get_cipher(
    enum crypto_algorithm algorithm,
    enum crypto_block_algorithm block_chaining_type
){
    char cipher_name[MAX_CIPHER_NAME_SIZE];

    // Build crypto algorithm 
    strncpy(cipher_name, cryptographic_algorithm_names[algorithm], cryptographic_algorithm_name_size[algorithm]);
    strncat(cipher_name, block_chaining_type_names[block_chaining_type], block_chaining_type_name_size[block_chaining_type]);

    // Get pointer from openssl by name
    return EVP_get_cipherbyname(cipher_name);
}



static int encrypt_decrypt(
    const uint8_t* in, 
    const uint32_t in_size, 
    const uint8_t* password, 
    enum crypto_algorithm algorithm, 
    enum crypto_block_algorithm block_chaining_type,
    uint8_t* out,
    uint32_t* out_size,
    int mode
){
    uint8_t* k;
    uint8_t* iv;
    uint32_t out_final_size, out_temporary_size;
    EVP_CIPHER_CTX* context;
    context = EVP_CIPHER_CTX_new();
    
    // Map encryption algorithm enum to special library type
    const EVP_CIPHER* cipher = get_cipher(algorithm, block_chaining_type);

    // Allocate space for key and iv
    k = malloc(sizeof(uint8_t*)*EVP_CIPHER_key_length(cipher));
    iv = malloc(sizeof(uint8_t*)*EVP_CIPHER_iv_length(cipher));

    // Initialize key and iv
    EVP_BytesToKey(cipher, EVP_md5(), NULL, password, strlen(password), 1, k, iv);

    // // Initialize context
    // EVP_CIPHER_CTX_init(context);

    // Encrypt/Decrypt
    // EVP_CypherInit_ex(context, cipher, NULL, k, iv, mode);
    EVP_CipherUpdate(context, out, &out_temporary_size, in, in_size);
    EVP_CipherFinal(context, out+out_temporary_size, &out_final_size);

    *out_size = out_temporary_size + out_final_size;

    // Free key and IV
    free(k);
    free(iv);

    // Clean context
    EVP_CIPHER_CTX_cleanup(context);

    return 0;
}