#include <reveal.h>
#include <structs.h>
#include <string.h>
#include <stdio.h>

#define HIDE_BUFFER_SIZE 256
#define FILE_SIZE_LENGTH 4

int reveal(Parameters params){

    // Decrpyt if data is encrypted using openssl
    // char* decrypted_payload;

    // char * stego_string = params.steg;
    // if (strcmp(stego_string, "lsb1") == 0)
    //     reveal_lsb1(decrypted_payload, params.out_file);
    // else if (strcmp(stego_string, "lsb4") == 0)
    //     reveal_lsb4(decrypted_payload, params.out_file);
    // else if (strcmp(stego_string, "lsbi") == 0)
    //     reveal_lsbi(decrypted_payload, params.out_file);
    // else {
    //     printf("stego parameter not supported. Use lsb1, lsb4 or lsbi\n");
    //     return -1;
    // }
    // return 0;

    char * stego_string = params.steg;
    void (*steg_function)(BMPFile bmp_file, FILE* out_file);

    if (strcmp(stego_string, "lsb1") == 0){
        steg_function = reveal_lsb1;
        if (params.bmp.size / 8 < (params.payload.size + FILE_SIZE_LENGTH + FILE_EXTENSION_LENGTH)){
            printf("BMP file not big enough for payload, min size of body required is %d\n", params.payload.size*8);
            return -1;
        }
    }
    else if (strcmp(stego_string, "lsb4") == 0){
        steg_function = reveal_lsb4;
        if (params.bmp.size / 2 < (params.payload.size + FILE_SIZE_LENGTH + FILE_EXTENSION_LENGTH)){
            printf("BMP file not big enough for payload, min size of body required is %d\n", params.payload.size*2);
            return -1;
        }
    }
    else if (strcmp(stego_string, "lsbi") == 0){
        steg_function = reveal_lsbi;
        // Size check
    }
    else {
        printf("stego parameter not supported. Use lsb1, lsb4 or lsbi\n");
        return -1;
    }

    // Encrypt using openssl if encryption was requested

    // If encrpytion was requested, check that bmp file is long enough for new payload to fit

    // steganograph: file_size || file || ".extension\0"

    // Hide the data [ file_size || file || ".extension\0" ] == [ 32 | m_size | 56(., 5 chars and 0) ]
    steg_function(params.bmp, params.out_file);
    return 0;
}

void reveal_lsb1(BMPFile bmp, FILE* out_file){


}

void reveal_lsb4(BMPFile bmp, FILE* out_file){

}

void reveal_lsbi(BMPFile bmp, FILE* out_file){

}