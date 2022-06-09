#ifndef _STEGOBMP_H_
#define _STEGOBMP_H_

#include <structs.h>

Parameters * parse_arguments(int argc, char *argv[]);
PayloadFile * get_payload_from_path(char * path);
BMPFile * get_bmp_from_path(char* path);
const char *get_filename_ext(const char *filename);
void free_params(Parameters * parameters);

#endif