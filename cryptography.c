#include <cryptography.h>
#include <string.h>

const char* cryptographic_algorithm_names[ENCRYPTION_ALGORITHMS_NUMBER] = {"aes-128-", "aes-192-", "aes-256-", "des-"};
const int cryptographic_algorithm_name_size[ENCRYPTION_ALGORITHMS_NUMBER] = {9, 9, 9, 5};

const char* block_chaining_type_names[BLOCK_CHAINING_TYPE_NUMBER] = {"ecb", "cfb", "ofb", "cbc"};
const int block_chaining_type_name_size[BLOCK_CHAINING_TYPE_NUMBER] = {3, 3, 3, 3};


int encrypt(char * payload, size_t payload_size, Parameters params, char * encrypted_data){
    enum crypto_algorithm enc_algo = get_encryption_algorithm((char *)params.enc_alg);
    enum crypto_block_algorithm block_algo = get_block_algorithm((char *)params.enc_mode);
    uint32_t encrypted_data_size;

    if(encrypted_data == NULL){
        printf("Encrypted data is null on encrypt, error\n");
        exit(1);
    }

    encrypt_decrypt((uint8_t *)payload, payload_size, params.password, enc_algo, block_algo, (uint8_t*)encrypted_data, &encrypted_data_size, 1);

    return encrypted_data_size;
}

int decrypt(FILE* encrypted_file, Parameters param, char * decrypted_data){
    // Levantar de archivo y poner en un buffer
    if (fseek(encrypted_file, 0, SEEK_END) != 0){
        printf("Error in fseek");
    }
    long ftell_result = ftell(encrypted_file);
    if (ftell_result == -1){
        printf("Error in ftell\n");
    }
    size_t in_size = ftell_result;
    
    fseek(encrypted_file, 0, SEEK_SET);
    // Set body pointer
    char * encrypted_data;
    encrypted_data = malloc(in_size +1);
    if(encrypted_data == NULL){
        printf("Malloc error\n");
        exit(1);
    }
    fread(encrypted_data, in_size, 1, encrypted_file);
    encrypted_data[in_size] = 0;



    enum crypto_algorithm enc_algo = get_encryption_algorithm((char *)param.enc_alg);
    enum crypto_block_algorithm block_algo = get_block_algorithm((char *)param.enc_mode);
    uint32_t decrypted_data_size;
    if(decrypted_data == NULL){
        printf("Malloc error\n");
        exit(1);
    }

    encrypt_decrypt((uint8_t*)encrypted_data, in_size, param.password, enc_algo, block_algo, (uint8_t*)decrypted_data, &decrypted_data_size, 0);

    free(encrypted_data);

    return decrypted_data_size;
}

enum crypto_block_algorithm get_block_algorithm(char * algorithm){
    if(strcmp(algorithm, "cbc")==0){
        return CBC;
    }else if(strcmp(algorithm, "ofb")==0){
        return OFB;
    }else if(strcmp(algorithm, "cfb")==0){
        return CFB;
    }else if(strcmp(algorithm, "ecb")==0){
        return ECB;
    }else{
        printf("Encryption mode not supported %s\n", algorithm);
        exit(1);
    }
}

enum crypto_algorithm get_encryption_algorithm(char * algorithm){
    if(strcmp(algorithm, "des")==0){
        return DES;
    }else if(strcmp(algorithm, "aes192")==0){
        return AES_192;
    }else if(strcmp(algorithm, "aes256")==0){
        return AES_256;
    }else if(strcmp(algorithm, "aes128")==0){
        return AES_128;
    }else{
        printf("Encryption algorithm not supported %s\n", algorithm);
        exit(1);
    }
}

const EVP_CIPHER* get_cipher(
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



int encrypt_decrypt(
    const uint8_t* in, 
    const uint32_t in_size, 
    const char* password, 
    enum crypto_algorithm algorithm, 
    enum crypto_block_algorithm block_chaining_type,
    uint8_t* out,
    uint32_t* out_size,
    int mode
){

    uint8_t* k;
    uint8_t* iv;
    int out_final_size, out_temporary_size;
    EVP_CIPHER_CTX* context;

    context = EVP_CIPHER_CTX_new();
    if(!context){
        printf("Context error\n");
        exit(1);
    }
    // Map encryption algorithm enum to special library type
    const EVP_CIPHER* cipher = get_cipher(algorithm, block_chaining_type);
    if (cipher == NULL){
        printf("Cipher is null\n");
    }

    // Allocate space for key and iv
    k = malloc(sizeof(uint8_t*) * EVP_CIPHER_key_length(cipher));
    iv = malloc(sizeof(uint8_t*) * EVP_CIPHER_iv_length(cipher));

    // Initialize key and iv
    if(!EVP_BytesToKey(cipher, EVP_sha256(), NULL, (uint8_t*)password, strlen((char *)password), 1, k, iv)){
        printf("EVP bytes to key error");
        exit(1);
    }

    // Encrypt/Decrypt
    if(EVP_CipherInit_ex(context, cipher, NULL, k, iv, mode)==0){
        printf("Intialization error");
        exit(1);
    }

    if(EVP_CipherUpdate(context, out, &out_temporary_size, in, in_size)==0){
        printf("Decryption error in update");
        exit(1);
    }
    int final_return;
    final_return = EVP_CipherFinal(context, out+out_temporary_size, &out_final_size);
    if(final_return == 0){
        printf("Decryption error in final");
        exit(1);
    }

    *out_size = out_temporary_size + out_final_size;
    EVP_CIPHER_CTX_free(context);

    // Free key and IV
    free(k);
    free(iv);


    return 0;
}
