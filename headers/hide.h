#ifndef _HIDE_H_
#define _HIDE_H_

#include <structs.h>

int hide(Parameters parameters);
void hide_lsb1(BMPFile bmp, char * payload, size_t payload_size, FILE* out_file);
void hide_lsb4(BMPFile bmp, char * payload, size_t payload_size, FILE* out_file);
void hide_lsbi(BMPFile bmp, char * payload, size_t payload_size, FILE* out_file);

#endif