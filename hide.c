#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hide.h>
#include <structs.h>
#include <cryptography.h>

#define HIDE_BUFFER_SIZE 256
#define FILE_SIZE_LENGTH 4
#define FILE_EXTENSION_LENGTH 8
#define INVERTED_BIT 0
#define SAME_BIT 1
#define DATA_BUFF_SIZE 1024 * 1024 // 1MB

uint8_t hide_buffer[HIDE_BUFFER_SIZE+1];


int hide(Parameters params){
    char * encrypted_data;
    int encrypted_data_size;
    if (params.encrypted){
        char * data_to_encrypt = malloc(FILE_SIZE_LENGTH + params.payload->size + strlen(params.payload->extension)+1);
        if(data_to_encrypt == NULL){
            perror("Malloc error in data to encrypt\n");
        }
        encrypted_data = malloc(DATA_BUFF_SIZE);
        if(encrypted_data == NULL){
            perror("Malloc error in encrypted data\n");
        }
        // Build a char*   payload_size||payload||extension
        // Get payload file size
        uint8_t p_size_string[FILE_SIZE_LENGTH + 1];
        fill_payload_size_str(p_size_string, params.payload->size);

        memcpy(data_to_encrypt, p_size_string, FILE_SIZE_LENGTH);
        memcpy(data_to_encrypt + FILE_SIZE_LENGTH, params.payload->body, params.payload->size);
        memcpy(data_to_encrypt + FILE_SIZE_LENGTH + params.payload->size, params.payload->extension, strlen(params.payload->extension) + 1);   

        encrypted_data_size = encrypt(data_to_encrypt, FILE_SIZE_LENGTH + params.payload->size + strlen(params.payload->extension) + 1, params, encrypted_data);
    

        free(data_to_encrypt);
    }
    
    

    // Check that bmp file is long enough for the payload to fit. If not throw error
    // Needs to fit [ file_size || file || ".extension\0" ] == [ 32 | m_size | 64(., 6 chars and 0) ]
    char * stego_string = params.steg;
    void (*steg_function)(BMPFile bmp_file, char* payload, size_t payload_size, FILE* out_file);
    char * data_to_hide;
    uint32_t payload_actual_size;
    if (params.encrypted) {
        data_to_hide = malloc(encrypted_data_size + FILE_SIZE_LENGTH);
        if (data_to_hide == NULL){
            printf("Malloc error\n");
            exit(1);
        }
        payload_actual_size = encrypted_data_size + FILE_SIZE_LENGTH;
        // Get encrypted adta size as 4 bytes
        uint8_t p_size_string[FILE_SIZE_LENGTH + 1];
        fill_payload_size_str(p_size_string, encrypted_data_size);
        memcpy(data_to_hide, p_size_string, FILE_SIZE_LENGTH);
        memcpy(data_to_hide + FILE_SIZE_LENGTH, encrypted_data, encrypted_data_size);
    } else {
        data_to_hide = malloc(DATA_BUFF_SIZE);
    
        uint8_t p_size_string[FILE_SIZE_LENGTH + 1];
        fill_payload_size_str(p_size_string, params.payload->size);
        
        memcpy(data_to_hide, p_size_string, FILE_SIZE_LENGTH);
        memcpy(data_to_hide + FILE_SIZE_LENGTH, params.payload->body, params.payload->size);
        memcpy(data_to_hide + FILE_SIZE_LENGTH + params.payload->size, params.payload->extension, strlen(params.payload->extension) + 1);


        payload_actual_size = FILE_SIZE_LENGTH + params.payload->size + strlen(params.payload->extension) + 1;
    }

    if (strcmp(stego_string, "lsb1") == 0){
        steg_function = hide_lsb1;
        if (params.bmp->size / 8 < payload_actual_size){
            printf("BMP file not big enough for payload, min size of body required is %d\n", params.payload->size*8);
            return -1;
        }
    }
    else if (strcmp(stego_string, "lsb4") == 0){
        steg_function = hide_lsb4;
        if (params.bmp->size / 2 < payload_actual_size){
            printf("BMP file not big enough for payload, min size of body required is %d\n", params.payload->size*2);
            return -1;
        }
    }
    else if (strcmp(stego_string, "lsbi") == 0){
        steg_function = hide_lsbi;
        if (params.bmp->size / 8 < payload_actual_size){
            printf("BMP file not big enough for payload, min size of body required is %d\n", params.payload->size*8);
            return -1;
        }
    }
    else {
        printf("stego parameter not supported. Use lsb1, lsb4 or lsbi\n");
        exit(1);
    }

    steg_function(*params.bmp, data_to_hide, payload_actual_size, params.out_file);
    free(data_to_hide);
    if (params.encrypted)
        free(encrypted_data);
    return 0;
}

void fill_payload_size_str(uint8_t* p_size_string, uint32_t p_size){
    // Divide int into bytes
    for (int i = 0; i < FILE_SIZE_LENGTH; i++){
        p_size_string[i] = (uint8_t) ((p_size >> (FILE_SIZE_LENGTH-i-1)*8) & 0x000000ff);
    }
    p_size_string[FILE_SIZE_LENGTH] = 0;
    return;
}


void hide_lsb1(BMPFile bmp, char * payload, size_t payload_size, FILE* out_file){
    // On each block replace least significant bit with message bit
    int out_file_size = 0;

    size_t header_size;
    // Copy original BMP Header
    if ((header_size = fwrite( bmp.header, 54, 1, out_file)) < 0){
        out_file_size+=54;
        printf("Error writing original BMP header to out file\n");
        exit(-1);
    }
    

    uint8_t mask = 0x01;
    uint8_t new_byte;
    int buff_pos = 0;


    uint8_t* byte_to_hide = payload;
    // Hide payload body
    for (int i=0; i < (payload_size) * 8; i++){
        uint8_t bit_to_add = (*(byte_to_hide) >> (7 - i % 8)) & mask;       // 0x00 or 0x01
        if (i % 8 == 7){     // Advance pointer if all bits were already concealed
            byte_to_hide++;
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
    if(fwrite(bmp.current_byte, bmp.size - (payload_size) * 8, 1, out_file) < 0){
        printf("Error writing remainder of bmp to out file\n");
        exit(-1);
    }
}


void hide_lsb4(BMPFile bmp, char * payload, size_t payload_size, FILE* out_file){
    // On each block replace 4 least significant bits with message bits

    size_t header_size;
    // Copy original BMP Header
    if ((header_size = fwrite( bmp.header, 54, 1, out_file)) < 0){
        printf("Error writing original BMP header to out file\n");
        exit(-1);
    }
    
    uint8_t mask = 0x0F;
    uint8_t new_byte;
    int buff_pos = 0;

   
    uint8_t* byte_to_hide = payload;
    uint8_t bits_to_add;
    // Hide payload body
    for (int i=0; i < (payload_size) * 2; i++){
        bits_to_add = (*(byte_to_hide) >> (4 - 4 * (i % 2))) & mask;       // 0x00 through 0x0F
        
        if (i % 2 == 1 ){     // Advance pointer if all bits were already concealed
            byte_to_hide++;
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
    if(fwrite(bmp.current_byte, bmp.size - (payload_size) * 2, 1, out_file) < 0){
        printf("Error writing remainder of bmp to out file\n");
        exit(-1);
    }
}


void hide_lsbi(BMPFile bmp, char * payload, size_t payload_size, FILE* out_file){
    size_t header_size;
    // Copy original BMP Header
    if ((header_size = fwrite( bmp.header, 54, 1, out_file)) < 0){
        printf("Error writing original BMP header to out file\n");
        exit(-1);
    }
    


    size_t pattern_count[4][2] = {{0,0},{0,0},{0,0},{0,0}}; //00, 01, 10, 11 counts. [1] is same, [0] is inverted
    uint8_t pattern_mask = 0x06;
    uint8_t mask = 0x01;
    

    uint8_t* byte_to_hide = payload;
    uint8_t new_byte;
    uint8_t bmp_pattern;
    uint8_t bmp_bit;
    uint8_t bit_to_add;

    for (int i=0; i < payload_size * 8; i++){
        bit_to_add = (*(byte_to_hide) >> (7 - i % 8)) & mask;       // 0x00 or 0x01
        if (i % 8 == 7){     // Advance pointer if all bits were already counted
            byte_to_hide++;
        }

        bmp_pattern = *(bmp.current_byte) & pattern_mask >> 1;
        bmp_bit = *(bmp.current_byte) & mask;
        pattern_count[bmp_pattern][bmp_bit-bit_to_add == 0] += 1;

        bmp.current_byte++;

    }

    bmp.current_byte = bmp.body;
    byte_to_hide = payload;
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


    for (int i=0; i < payload_size * 8; i++){
        bit_to_add = (*(byte_to_hide) >> (7 - i % 8)) & mask;       // 0x00 or 0x01
        if (i % 8 == 7){     // Advance pointer if all bits were already concealed
            byte_to_hide++;
        }

        bmp_pattern = *(bmp.current_byte) & pattern_mask >> 1;
        // If got 0 and shouldnt invert or got 1 and should invert
        if ((bit_to_add == 0x00 && !must_invert[bmp_pattern]) || (bit_to_add == 0x01 && must_invert[bmp_pattern]))
            new_byte = *(bmp.current_byte) & (~mask); // bmp.current_byte & 1111 1110 => put 0
        else
            new_byte = *(bmp.current_byte) | mask; // bmp.current_byte | 0000 0001 => put 1

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
    if(fwrite(bmp.current_byte, bmp.size - (4+payload_size) * 8, 1, out_file) < 0){
        printf("Error writing remainder of bmp to out file\n");
        exit(-1);
    }
    
}