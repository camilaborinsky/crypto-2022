#include <reveal.h>
#include <structs.h>
#include <string.h>
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#define REVEAL_BUFFER_SIZE 256
#define FILE_SIZE_LENGTH 4
#define FILE_EXTENSION_LENGTH 8

char out_block[REVEAL_BUFFER_SIZE];
char extension[FILE_EXTENSION_LENGTH];

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
    }
    else if (strcmp(stego_string, "lsb4") == 0){
        steg_function = reveal_lsb4;
    }
    else if (strcmp(stego_string, "lsbi") == 0){
        steg_function = reveal_lsbi;
    }
    else {
        printf("stego parameter not supported. Use lsb1, lsb4 or lsbi\n");
        return -1;
    }

    // Encrypt using openssl if encryption was requested

    // If encrpytion was requested, check that bmp file is long enough for new payload to fit

    // steganograph: file_size || file || ".extension\0"

    // Hide the data [ file_size || file || ".extension\0" ] == [ 32 | m_size | 56(., 5 chars and 0) ]
    steg_function(*params.bmp, params.out_file);
    int new_file_name_size = strlen(params.out_file_name)+strlen(extension) +1;
    char new_file_name [new_file_name_size];
    strcpy(new_file_name, params.out_file_name);
    strcat(new_file_name, extension);
    new_file_name[new_file_name_size-1] = 0;
    rename(params.out_file_name, new_file_name);
    return 0;
}

void reveal_lsb1(BMPFile bmp, FILE* out_file){ 
    printf("Inside reveal lsb1\n");
    uint8_t mask = 0x01;
    uint8_t* curr_byte = bmp.body;
    uint8_t rebuilding_byte = 0x00;
    int out_buff_pos = 0;
    int ext_buff_pos = 0;
    uint32_t real_size = 0;
    

    // Reading payload real size
    for (int i = 0; i < 8 * FILE_SIZE_LENGTH; i++){
        uint8_t bit_to_reveal = *curr_byte & mask;
        curr_byte += 1;
        
        real_size = real_size << 1;
        real_size = real_size | bit_to_reveal;
        //printf("Leading text "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte));
        // printf("As int: %d\n", real_size);
        // printf("m: "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(real_size>>24), BYTE_TO_BINARY(real_size>>16), BYTE_TO_BINARY(real_size>>8), BYTE_TO_BINARY(real_size));
    }

    printf("Real size: %d\n", real_size);


    // Reading hidden data and writing to outfile
    for(int i = 0; i < real_size*8 ; i++){ //TODO chequear
        uint8_t bit_to_reveal = *curr_byte & mask;
        //printf("Current byte: %c\n", curr_byte);
        //printf("Current bit to reveal: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte));
        curr_byte += 1;
        
        rebuilding_byte = rebuilding_byte << 1;
        rebuilding_byte = rebuilding_byte | bit_to_reveal;
        if (i % 8 == 7 && i != 0){
            // Write to buffer
            out_block[out_buff_pos%256] = rebuilding_byte;
            out_buff_pos++;
            if(out_buff_pos%256 == 0){
                // printf("%s", out_block);
                fwrite(out_block, REVEAL_BUFFER_SIZE, 1, out_file);
            }
            rebuilding_byte = 0x00;   
        }
    }
    // Write remainder in buffer to out file
    fwrite(out_block, out_buff_pos+1, 1, out_file);
    rebuilding_byte = 0x00;   
    
    // Reading file extension
    int finished = 0;
    for(int i = 0; !finished ; i++){
        uint8_t bit_to_reveal = *curr_byte & mask;
        curr_byte += 1;

        rebuilding_byte = rebuilding_byte << 1;
        rebuilding_byte = rebuilding_byte | bit_to_reveal;

        if (i % 8 == 7){
            // Check for null termination
            if (rebuilding_byte == 0x00)
                finished = 1;

            // Write to buffer
            extension[ext_buff_pos] = rebuilding_byte;
            ext_buff_pos++;
            rebuilding_byte = 0x00;   
        }
    }
    printf("\nExtension: %s\n", extension);
}

void reveal_lsb4(BMPFile bmp, FILE* out_file){

}

void reveal_lsbi(BMPFile bmp, FILE* out_file){

}