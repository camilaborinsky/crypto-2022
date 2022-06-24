#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hide.h>
#include <structs.h>

#define HIDE_BUFFER_SIZE 256
#define FILE_SIZE_LENGTH 4
#define FILE_EXTENSION_LENGTH 8
#define INVERTED_BIT 0
#define SAME_BIT 1

uint8_t hide_buffer[HIDE_BUFFER_SIZE+1];


int hide(Parameters params){
    hide_buffer[HIDE_BUFFER_SIZE] = 0;

    // Check that bmp file is indeed bmp looking at extension. Check no double extension (.bmp.txt)
    // MOVE THIS CHECK TO WHEREVER BMP FILE IS BEING OPENED
    /*
    char * ext = strrchr(params.bmp, '.');
    if (strcmp(ext, ".bmp") != 0){
        printf("File passed as bmp is not bmp");
        exit(-1);
    }
    */

    // Check that bmp file is long enough for the payload to fit. If not throw error
    // Needs to fit [ file_size || file || ".extension\0" ] == [ 32 | m_size | 64(., 6 chars and 0) ]
    char * stego_string = params.steg;
    void (*steg_function)(BMPFile bmp_file, PayloadFile payload, FILE* out_file);

    if (strcmp(stego_string, "lsb1") == 0){
        steg_function = hide_lsb1;
        if (params.bmp->size / 8 < (params.payload->size + FILE_SIZE_LENGTH + FILE_EXTENSION_LENGTH)){
            printf("BMP file not big enough for payload, min size of body required is %d\n", params.payload->size*8);
            return -1;
        }
    }
    else if (strcmp(stego_string, "lsb4") == 0){
        steg_function = hide_lsb4;
        if (params.bmp->size / 2 < (params.payload->size + FILE_SIZE_LENGTH + FILE_EXTENSION_LENGTH)){
            printf("BMP file not big enough for payload, min size of body required is %d\n", params.payload->size*2);
            return -1;
        }
    }
    else if (strcmp(stego_string, "lsbi") == 0){
        steg_function = hide_lsbi;
        // Size check
    }
    else {
        printf("stego parameter not supported. Use lsb1, lsb4 or lsbi\n");
        exit(1);
    }

    // Encrypt using openssl if encryption was requested

    // If encrpytion was requested, check that bmp file is long enough for new payload to fit

    // steganograph: file_size || file || ".extension\0"

    // Hide the data [ file_size || file || ".extension\0" ] == [ 32 | m_size | 56(., 5 chars and 0) ]
    steg_function(*params.bmp, *params.payload, params.out_file);
    return 0;
}

void fill_payload_size_str(uint8_t* p_size_string, uint32_t p_size){
    // Divide int into bytes
    for (int i = 0; i < FILE_SIZE_LENGTH; i++){
        p_size_string[i] = (uint8_t) ((p_size >> (FILE_SIZE_LENGTH-i-1)*8) & 0xff);
    }

    p_size_string[FILE_SIZE_LENGTH] = 0;
    return;
}


void hide_lsb1(BMPFile bmp, PayloadFile payload, FILE* out_file){
    // On each block replace least significant bit with message bit
    int out_file_size = 0;

    size_t header_size;
    // Copy original BMP Header
    if ((header_size = fwrite( bmp.header, 54, 1, out_file)) < 0){
        out_file_size+=54;
        printf("Error writing original BMP header to out file\n");
        exit(-1);
    }
    
    printf("Header copied size (fprintf result): %ld\n", header_size);

    uint8_t mask = 0x01;
    uint8_t new_byte;
    int buff_pos = 0;

    // Get payload file size
    uint8_t p_size_string[FILE_SIZE_LENGTH + 1];
    fill_payload_size_str(p_size_string, payload.size);

    uint8_t* byte_to_hide = p_size_string;
    printf("Payload size is: %d\n", payload.size);
    printf("As string: %s\n", p_size_string);
    printf("Hiding payload size\n");
    // Hide payload body
    for (int i=0; i < (payload.size + FILE_SIZE_LENGTH + strlen(payload.extension) + 1) * 8; i++){
        uint8_t bit_to_add = (*(byte_to_hide) >> (7 - i % 8)) & mask;       // 0x00 or 0x01
        // printf("i===%d\n", i);
        if (i % 8 == 7){     // Advance pointer if all bits were already concealed
            byte_to_hide++;
            if (i == FILE_SIZE_LENGTH * 8 - 1){
                byte_to_hide = payload.body;    // Done hiding payload size, now hide body
                printf("Hiding payload body\n");
            }
            else if (i == (FILE_SIZE_LENGTH + payload.size) * 8 - 1){
                byte_to_hide = (uint8_t*) payload.extension;    // Done hiding payload body, now hide extension
                printf("Hiding payload extension\n");
            }
        }

        if (bit_to_add == 0x00)
            new_byte = *(bmp.current_byte) & (~mask);
        else
            new_byte = *(bmp.current_byte) | mask;
        bmp.current_byte++;

        hide_buffer[buff_pos++] = new_byte;

        if (buff_pos == HIDE_BUFFER_SIZE){
            //Write to out file
            if (fwrite(hide_buffer,HIDE_BUFFER_SIZE, 1, out_file) < 0){
                out_file_size+=HIDE_BUFFER_SIZE;
                printf("Error writing to out file\n");
                exit(-1);
            }
            //printf("Just wrote to out_file, buffer's contents were: %s\n", hide_buffer);
            buff_pos = 0;
        }
    }

    // Write remainder in hide_buffer to out file
    if (buff_pos != 0){
        hide_buffer[buff_pos++] = 0;
        if (fwrite(hide_buffer, buff_pos - 1, 1, out_file) < 0){ //
            printf("Error writing remainder of hide_buffer to out file\n");
            exit(-1);
        }
    }
    
    // Write remainder of original bmp as is
    if(fwrite(bmp.current_byte, bmp.size - (payload.size + FILE_SIZE_LENGTH + strlen(payload.extension)+1) * 8, 1, out_file) < 0){
    // if (fprintf(out_file, "%s", bmp.current_byte) < 0){
        printf("Error writing remainder of bmp to out file\n");
        exit(-1);
    }
}


void hide_lsb4(BMPFile bmp, PayloadFile payload, FILE* out_file){
    // On each block replace 4 least significant bits with message bits

    size_t header_size;
    // Copy original BMP Header
    if ((header_size = fwrite( bmp.header, 54, 1, out_file)) < 0){
        printf("Error writing original BMP header to out file\n");
        exit(-1);
    }
    
    printf("Header copied size (fprintf result): %ld\n", header_size);

    uint8_t mask = 0x0F;
    uint8_t new_byte;
    int buff_pos = 0;

    // Get payload file size
    uint8_t p_size_string[FILE_SIZE_LENGTH + 1];
    fill_payload_size_str(p_size_string, payload.size);

    uint8_t* byte_to_hide = p_size_string;
    uint8_t bits_to_add;
    // Hide payload body
    for (int i=0; i < (payload.size + FILE_SIZE_LENGTH + strlen(payload.extension)+1) * 2; i++){
        bits_to_add = (*(byte_to_hide) >> (4 - 4 * (i % 2))) & mask;       // 0x00 through 0x0F
        
        if (i % 2 == 1 ){     // Advance pointer if all bits were already concealed
            byte_to_hide++;
            if (i == FILE_SIZE_LENGTH * 2 - 1) 
                byte_to_hide = payload.body;    // Done hiding payload size, now hide body
            else if (i == (FILE_SIZE_LENGTH + payload.size) * 2  - 1)
                byte_to_hide = (uint8_t*) payload.extension;    // Done hiding payload body, now hide extension
        }

        new_byte = (*(bmp.current_byte) & (~mask)) | bits_to_add;
        bmp.current_byte++;

        // Write byte to file or (better) to buffer to write more than a single byte at a time
        hide_buffer[buff_pos++] = new_byte;

        if (buff_pos == HIDE_BUFFER_SIZE){
            //Write to out file
            if (fwrite(hide_buffer,HIDE_BUFFER_SIZE, 1, out_file) < 0){
                printf("Error writing to out file\n");
                exit(-1);
            }
            buff_pos = 0;
        }
    }

    // Write remainder in hide_buffer to out file
    if (buff_pos != 0){
        hide_buffer[buff_pos++] = 0;
        if (fwrite(hide_buffer, buff_pos - 1, 1, out_file) < 0){ //
            printf("Error writing remainder of hide_buffer to out file\n");
            exit(-1);
        }
    }
    
    // Write remainder of original bmp as is
    if(fwrite(bmp.current_byte, bmp.size - (payload.size + FILE_SIZE_LENGTH + strlen(payload.extension)+1) * 2, 1, out_file) < 0){
        printf("Error writing remainder of bmp to out file\n");
        exit(-1);
    }
}


void hide_lsbi(BMPFile bmp, PayloadFile payload, FILE* out_file){
    size_t header_size;
    // Copy original BMP Header
    if ((header_size = fwrite( bmp.header, 54, 1, out_file)) < 0){
        printf("Error writing original BMP header to out file\n");
        exit(-1);
    }
    
    printf("Header copied size (fprintf result): %ld\n", header_size);


    size_t pattern_count[4][2] = {{0,0},{0,0},{0,0},{0,0}}; //00, 01, 10, 11 counts. [1] is same, [0] is inverted
    uint8_t pattern_mask = 0x06;
    uint8_t mask = 0x01;
    uint8_t p_size_string[FILE_SIZE_LENGTH + 1];
    fill_payload_size_str(p_size_string, payload.size);
    

    uint8_t* byte_to_hide = p_size_string;
    uint8_t new_byte;
    uint8_t bmp_pattern;
    uint8_t bmp_bit;
    uint8_t bit_to_add;

    for (int i=0; i < (payload.size + FILE_SIZE_LENGTH + strlen(payload.extension) + 1) * 8; i++){
        bit_to_add = (*(byte_to_hide) >> (7 - i % 8)) & mask;       // 0x00 or 0x01
        if (i % 8 == 7){     // Advance pointer if all bits were already counted
            byte_to_hide++;
            if (i == FILE_SIZE_LENGTH * 8 - 1){
                byte_to_hide = payload.body;    // Done counting payload size, now hide body
                printf("Hiding payload body\n");
            }
            else if (i == (FILE_SIZE_LENGTH + payload.size) * 8 - 1){
                byte_to_hide = (uint8_t*) payload.extension;    // Done counting payload body, now count extension
                printf("Hiding payload extension\n");
            }
        }

        bmp_pattern = *(bmp.current_byte) & pattern_mask >> 1;
        bmp_bit = *(bmp.current_byte) & mask;
        pattern_count[bmp_pattern][bmp_bit-bit_to_add == 0] += 1;

        bmp.current_byte++;

    }

    bmp.current_byte = bmp.body;
    byte_to_hide = p_size_string;
    int buff_pos = 0;


    size_t must_invert[4] = {0,0,0,0}; //00, 01, 10, 11. 1 if we have to invert, 0 if not
    //Hide whether we inverted bits or not
    for (int i=0; i < 4; i++){
        if(pattern_count[i][INVERTED_BIT] > pattern_count[i][SAME_BIT]){
            new_byte = *(bmp.current_byte) | (mask); // 0000 0001
            must_invert[i] = 1;
        } else {
            new_byte = *(bmp.current_byte) & ~(mask); //1111 1110
            must_invert[i] = 0;
        }
        hide_buffer[buff_pos++] = new_byte;
        bmp.current_byte++;
    }


    for (int i=0; i < (payload.size + FILE_SIZE_LENGTH + strlen(payload.extension) + 1) * 8; i++){
        bit_to_add = (*(byte_to_hide) >> (7 - i % 8)) & mask;       // 0x00 or 0x01
        // printf("i===%d\n", i);
        if (i % 8 == 7){     // Advance pointer if all bits were already concealed
            byte_to_hide++;
            if (i == FILE_SIZE_LENGTH * 8 - 1){
                byte_to_hide = payload.body;    // Done hiding payload size, now hide body
                printf("Hiding payload body\n");
            }
            else if (i == (FILE_SIZE_LENGTH + payload.size) * 8 - 1){
                byte_to_hide = (uint8_t*) payload.extension;    // Done hiding payload body, now hide extension
                printf("Hiding payload extension\n");
            }
        }

        bmp_pattern = *(bmp.current_byte) & pattern_mask >> 1;
        // Si tengo un cero y lo dejo igual o tengo un uno y lo tengo que invertir
        if ((bit_to_add == 0x00 && !must_invert[bmp_pattern]) || (bit_to_add == 0x01 && must_invert[bmp_pattern]))
            new_byte = *(bmp.current_byte) & (~mask); // bmp.current_byte & 1111 1110 => dejo cero
        else
            new_byte = *(bmp.current_byte) | mask; // bmp.current_byte | 0000 0001 => dejo un uno

        bmp.current_byte++;

        hide_buffer[buff_pos++] = new_byte;

       
        if (buff_pos == HIDE_BUFFER_SIZE){
            //Write to out file
            if (fwrite(hide_buffer,HIDE_BUFFER_SIZE, 1, out_file) < 0){
                printf("Error writing to out file\n");
                exit(-1);
            }
            buff_pos = 0;
        }
    }
    
    // Write remainder in hide_buffer to out file
    if (buff_pos != 0){
        hide_buffer[buff_pos++] = 0;
        if (fwrite(hide_buffer, buff_pos - 1, 1, out_file) < 0){ //
            printf("Error writing remainder of hide_buffer to out file\n");
            exit(-1);
        }
    }
    
    // Write remainder of original bmp as is
    if(fwrite(bmp.current_byte, bmp.size - (4+payload.size + FILE_SIZE_LENGTH + strlen(payload.extension)+1) * 8, 1, out_file) < 0){
        printf("Error writing remainder of bmp to out file\n");
        exit(-1);
    }
    
}