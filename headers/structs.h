#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdint.h>
#include <stdio.h>

#define FILE_EXTENSION_LENGTH 8


typedef struct BMPHeader {
    char file_signature[2];             // The characters ‘B’ and ‘M’
    uint32_t file_size;                 // Size in bytes of the file including headers and pixel data
    uint8_t reserved[4];                // Unused
    uint32_t data_offset;               // Offset in the file where the pixel data is stored
    uint32_t header_size;               // The header is fixed size: 40 bytes
    uint32_t image_width;               // Width of the image in pixels
    uint32_t image_height;              // Height of the image in pixels
    uint16_t image_color_planes;        // Number of color planes (must be 1)
    uint16_t image_bpp;                 // Number of bits per pixel
    uint32_t image_compression;         // Compression method
    uint32_t image_size;                // Can be 0 if image is not compressed, otherwise is the size of the compressed image
    uint32_t image_pixels_per_meter_X;  // Horizontal resolution in pixels per meter
    uint32_t image_pixels_per_meter_Y;  // Vertical resolution in pixels per meter
    uint32_t image_colors_used;         // Number used colors
    uint32_t image_important_colors;    // Number of important color. Can be 0 If all colors are important
} BMPHeader;

//bytes totales - cuantos tengo hasta el archivo
//file_size - data_offset = SIZE QUE NOS SIRVE

typedef struct {
    FILE* file;
    /*
    char * buffer;
    uint32_t complete_size;
    */
    BMPHeader * header;
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



//TODO: function ptr
typedef struct {
    int hide;           // 1 if hiding, 0 if revealing
    PayloadFile* payload;       // file we are hiding
    BMPFile* bmp;
    FILE* out_file;
    char* steg;         // Maybe make func ptr but thing is hide and reveal stegs probably receive different params 
    char* enc_alg;
    char* enc_mode;
    char* password;
} Parameters;

#endif