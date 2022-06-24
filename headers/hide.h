#ifndef _HIDE_H_
#define _HIDE_H_

#include <structs.h>

int hide(Parameters parameters);
void hide_lsb1(BMPFile bmp, PayloadFile payload, FILE* out_file);
void hide_lsb4(BMPFile bmp, PayloadFile payload, FILE* out_file);
void hide_lsbi(BMPFile bmp, PayloadFile payload, FILE* out_file);

#endif