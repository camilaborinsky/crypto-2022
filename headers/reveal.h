#ifndef _REVEAL_H_
#define _REVEAL_H_

#include <structs.h>
#include <stdio.h>

int reveal(Parameters parameters);
void reveal_lsbi(BMPFile decrypted_bmp, FILE* out_file, int n, int encrypted);
void reveal_lsbn(BMPFile decrypted_bmp, FILE* out_file, int n, int encrypted);
void separate_decrypted_data(char* decrypted_data, size_t decrypted_data_size, FILE* out_file);
#endif