#include <reveal.h>
#include <structs.h>
#include <string.h>
#include <stdio.h>

int reveal(Parameters params){

    // Decrpyt if data is encrypted using openssl
    char* decrypted_payload;

    char * stego_string = params.steg;
    if (strcmp(stego_string, "lsb1") == 0)
        reveal_lsb1(decrypted_payload, params.out_file);
    else if (strcmp(stego_string, "lsb4") == 0)
        reveal_lsb4(decrypted_payload, params.out_file);
    else if (strcmp(stego_string, "lsbi") == 0)
        reveal_lsbi(decrypted_payload, params.out_file);
    else {
        printf("stego parameter not supported. Use lsb1, lsb4 or lsbi\n");
        return -1;
    }
    return 0;
}

void reveal_lsb1(char* decrypted_payload, FILE* out_file){

}

void reveal_lsb4(char* decrypted_payload, FILE* out_file){

}

void reveal_lsbi(char* decrypted_payload, FILE* out_file){

}