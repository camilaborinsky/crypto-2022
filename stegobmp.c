#include <unistd.h>
#include <getopt.h>
#include <stegobmp.h>
#include <hide.h>
#include <reveal.h>
#include <structs.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_EXTENSION_LENGTH 8
#define HEADER_SIZE 54

int main(int argc, char *argv[])
{
    // Read arguments
    Parameters* params = parse_arguments(argc, argv);

    int return_value = 0;
    if(params->encrypted && params->password == NULL ){
        printf("Missing cryptographic password or block chaining mode\n");
        return_value = 1;
    }
    if(params->steg == NULL){
        printf("Steg algorithm is required\n");
        return_value = 1;
    }
    if(return_value == 0){
        if (params->hide == 1)
            return_value = hide(*params);
        else
            return_value = reveal(*params);
    }
    free_params(params);
    return return_value;
}

Parameters * parse_arguments(int argc, char *argv[]){
    Parameters * params = malloc(sizeof(Parameters));
    if (params == NULL){
        printf("Malloc error\n");
        exit(1);
    }
    params->encrypted = 0;
    params->password = NULL;
    params->enc_alg = NULL;
    params->enc_mode = NULL;
    params->steg = NULL;
    params->out_file_name = NULL;
    char* opts = "xep:o:i:s:a:m:k:";
    int c;
    static struct option long_options[] =
        {
            {"extract",     no_argument, 0, 'x'},
            {"embed",     no_argument, 0, 'e'},
            {"p",  required_argument, 0, 'p'},
            {"out",  required_argument, 0, 'o'},
            {"in",  required_argument, 0, 'i'},
            {"steg",  required_argument, 0, 's'},
            {"a",    required_argument, 0, 'a'},
            {"m",    required_argument, 0, 'm'},
            {"pass",    required_argument, 0, 'k'},
            {0, 0, 0, 0}
        };
    int option_index = 0;

    while (( c = getopt_long (argc, argv, opts,long_options, &option_index))>=0){
        switch (c)
            {
        
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                break;

            case 'x':
                params->hide = 0;
                break;

            case 'e':
                params->hide = 1;
                break;

            case 'p':{
                params->bmp = get_bmp_from_path(optarg);
            }break;

            case 'o': ; // Semicolon to avoid label error
                FILE* out_file = fopen(optarg, "w");
                if (out_file == NULL){
                    printf("Error opening file\n");
                    exit(1);
                }
                params-> out_file_name = optarg;
                params->out_file = out_file;
                break;

            case 'i':
                params->payload = get_payload_from_path(optarg);
                break;

            case 's':
                params->steg = optarg;
                break;

            case 'a':
                params->encrypted = 1;
                params->enc_alg = optarg;
                break;

            case 'm':
                params->encrypted = 1;
                params->enc_mode = optarg;
                break;

            case 'k':
                params->encrypted = 1;
                params->password = optarg;
                break;
            default:
                abort ();
            }
        }
    return params;
        
    
}

PayloadFile * get_payload_from_path(char* path){
    PayloadFile * payload_struct = malloc(sizeof(PayloadFile));
    if(payload_struct == NULL){
        printf("Malloc error\n");
        exit(1);
    }

    FILE* payload_file = fopen(path, "r");
    if (payload_file == NULL){
        printf("Error opening payload_file");
        exit(1);
    }
    payload_struct->file = payload_file;
    
    const char* ext = get_filename_ext(path);
    if (strlen(ext) > FILE_EXTENSION_LENGTH){
        printf("Payload extension length too long, should be less than %d", FILE_EXTENSION_LENGTH);
        exit(1);
    }
    strcpy(payload_struct->extension, ext);
    
    // Get size
    if (fseek(payload_file, 0, SEEK_END) != 0){
        printf("Error in fseek");
    }
    long ftell_result = ftell(payload_file);
    if (ftell_result == -1){
        printf("Error in ftell\n");
    }
    payload_struct->size = ftell_result;
    fseek(payload_file, 0, SEEK_SET);

    // Set body pointer
    payload_struct->body = malloc( payload_struct-> size +1);
    if(payload_struct->body == NULL){
        printf("Malloc error\n");
        exit(1);
    }
    fread(payload_struct->body, 1, payload_struct-> size, payload_file);
    payload_struct->body[payload_struct->size] = 0;

    fclose(payload_file);
    
    return payload_struct;
}

BMPFile * get_bmp_from_path(char* path){

    // Check that cover file is actually bmp
    const char* cover_ext = get_filename_ext(path);
    if (strcmp(cover_ext, ".bmp") != 0){
        printf("Cover file should be .bmp, received: %s\n", cover_ext);
        exit(1);
    }

    BMPFile * bmp = malloc(sizeof(BMPFile));
    if(bmp == NULL){
        printf("Malloc error\n");
        exit(1);
    }

    // Open file
    FILE* bmp_file = fopen(path, "r");
    if(bmp_file == NULL ){
        printf("Error opening bmp file");
        exit(1);
    }
    bmp->file = bmp_file;

    // Set header pointer
    bmp->header = malloc(HEADER_SIZE + 1);
    if (bmp->header == NULL){
        printf("Malloc error\n");
        exit(1);
    }
    fread(bmp->header, HEADER_SIZE, 1, bmp_file);
    bmp->header[HEADER_SIZE] = 0;
    
    // Check that bmp is not compressed, if it is then exit
    uint8_t compression[4] = {-1,-1,-1,-1};
    memcpy(compression , bmp->header + 30 , 4);

    if(compression[0] != 0 && compression[1] != 0 && compression[2] != 0 && compression[3] != 0){
        printf("Compression: %X %X %X %X\n" , compression[0] ,compression[1], compression[2], compression[3]);
        printf("Image cannot be compressed!\n");
        exit(1);
    }
    
    // TODO volar la estructura BMPHeader "real" si no la usamos

    // Get size
    if (fseek(bmp_file, 0, SEEK_END) != 0){
        printf("Error in fseek");
    }
    long ftell_result = ftell(bmp_file);
    if (ftell_result == -1){
        printf("Error in ftell\n");
    }
    bmp->size = ftell_result - HEADER_SIZE;
    fseek(bmp_file, 0, SEEK_SET);
    
    // Set body pointer
    bmp->body = malloc( bmp-> size +1);
    if (bmp->body == NULL){
        printf("Malloc error\n");
        exit(1);
    }
    fseek(bmp_file, HEADER_SIZE, SEEK_SET);
    fread(bmp->body, 1, bmp-> size, bmp_file);
    bmp->body[bmp->size] = 0;
    
    // Set current byte pointer to initial position
    bmp->current_byte = bmp->body;
    fclose(bmp_file);
    return bmp;
    
}

void free_params(Parameters * params){
    free(params->bmp->header);
    free(params->bmp->body);
    free(params->bmp);
    if (params->hide){
        free(params->payload->body);
        free(params->payload);
    } else {
        fclose(params->out_file);
    }
    free(params);
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot;
}