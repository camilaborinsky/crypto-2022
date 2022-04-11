#ifndef _REVEAL_H_
#define _REVEAL_H_

#include "structs.h"
#include <stdio.h>

int reveal(Parameters parameters);
void reveal_lsb1(char* decrypted_payload, FILE* out_file);
void reveal_lsb4(char* decrypted_payload, FILE* out_file);
void reveal_lsbi(char* decrypted_payload, FILE* out_file);

#endif