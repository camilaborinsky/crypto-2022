#ifndef _REVEAL_H_
#define _REVEAL_H_

#include <structs.h>
#include <stdio.h>

int reveal(Parameters parameters);
void reveal_lsb1(BMPFile decrypted_bmp, FILE* out_file);
void reveal_lsb4(BMPFile decrypted_bmp, FILE* out_file);
void reveal_lsbi(BMPFile decrypted_bmp, FILE* out_file);

#endif