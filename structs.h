#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdint.h>
#include <stdio.h>

typedef struct 
{
    uint32_t* header;
    uint32_t* body;
    uint32_t* current_byte;
} BMPFile;

typedef void (*steg_function)();

typedef struct 
{
    int hide;           // 1 if hiding, 0 if revealing
    FILE* in_file;       // file we are hiding
    FILE* bmp_file;
    FILE* out_file;
    steg_function steg;
    char* enc_alg;
    char* enc_mode;
    char* password;
} Parameters;

#endif