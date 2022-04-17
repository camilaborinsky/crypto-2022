#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdint.h>
#include <stdio.h>

#define FILE_EXTENSION_LENGTH 8

typedef struct {
    FILE* file;
    uint8_t* header;
    uint8_t* body;
    uint8_t* current_byte;
    uint32_t size;  //Not counting header, actual size we can work with (in bytes)
} BMPFile;

typedef struct {
    FILE* file;
    char extension[FILE_EXTENSION_LENGTH];     // eg. '.png\0'
    uint8_t* body;
    uint32_t size;
} PayloadFile;
/* To get size
fseek(file, 0, SEEK_END);
bmp_file.size = ftell(file);
fseek(file, 0, SEEK_SET);
*/

typedef struct {
    int hide;           // 1 if hiding, 0 if revealing
    PayloadFile payload;       // file we are hiding
    BMPFile bmp;
    FILE* out_file;
    char* steg;         // Maybe make func ptr but thing is hide and reveal stegs probably receive different params
    char* enc_alg;
    char* enc_mode;
    char* password;
} Parameters;

#endif