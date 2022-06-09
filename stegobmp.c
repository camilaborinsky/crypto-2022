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

int main(int argc, char *argv[])
{
    // Read arguments
    printf("%d\n", argc);
    Parameters* params = parse_arguments(argc, argv);

    printf("BMP file : %d\n", params->bmp->size);
    printf("Payload file : %d\n", params->bmp->size);

    int return_value = 0;

    if (params->hide == 1)
        return_value = hide(*params);
    else
        return_value = reveal(*params);

    free_params(params);
    return return_value;
}

Parameters * parse_arguments(int argc, char *argv[]){
    Parameters * params = malloc(sizeof(Parameters));
    char* opts = "xepoisamk";
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
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'x':
                params->hide = 0;
                break;

            case 'e':
                params->hide = 1;
                break;

            case 'p':
                printf("\nBuena suerte\n");
                //puts(optarg);
                printf("Here are first 10 chars only: %.10s\n", optarg);
                printf("\nVolvimos\n");
                params->bmp = get_bmp_from_path(optarg);
                break;

            case 'o': ; // Semicolon to avoid label error
                FILE* out_file = fopen(optarg, "w");
                if (out_file == NULL){
                     printf("Error opening out_file");
                     exit(1);
                }
                params->out_file = out_file;
                break;

            case 'i':
                params->payload = get_payload_from_path(optarg);
                break;

            case 's':
                params->steg = optarg;
                break;

            case 'a':
                params->enc_alg = optarg;
                break;

            case 'm':
                params->enc_mode = optarg;
                break;

            case 'k':
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

    FILE* payload_file = fopen(path, "r");
    if (payload_file == NULL){
        printf("Error opening payload_file");
        exit(1);
    }
    payload_struct->file = payload_file;
    
    char* ext = get_filename_ext(path);
    if (strlen(ext) > FILE_EXTENSION_LENGTH){
        printf("Payload extension length too long, should be less than %d", FILE_EXTENSION_LENGTH);
        exit(1);
    }
    strcpy(payload_struct->extension, ext);
    
    // Get size
    fseek(payload_file, 0, SEEK_END);
    payload_struct->size = ftell(payload_file);
    fseek(payload_file, 0, SEEK_SET);

    /*
    // Read file and get pointer to start
    int bytes_read = fscanf(payload_file, "%s", payload_struct->body);
    if (bytes_read != payload_struct->size){
        printf("Error on payload parse. N° of bytes read by fscanf dont match file size");
        exit(1);
    }
    */

    //set body pointer
    payload_struct->body = malloc( payload_struct-> size * sizeof(uint8_t));
    fread(payload_struct->body, 1, payload_struct-> size, payload_file);

    fclose(payload_file);
    
    return payload_struct;
}

BMPFile * get_bmp_from_path(char* path){
    BMPFile * bmp = malloc(sizeof(BMPFile));
    printf("%s\n", path);
    FILE* bmp_file = fopen(path, "r");
    if(bmp_file == NULL ){
        printf("Error opening bmp file");
        exit(1);
    }
    bmp->file = bmp_file;

    //set header pointer
    bmp->header = malloc(sizeof(BMPHeader));
    fread(bmp->header, sizeof(BMPHeader), 1, bmp_file);

    //set file size
    bmp->size = (bmp->header->file_size - bmp->header->data_offset);

    //set body pointer
    bmp->body = malloc( bmp-> size * sizeof(uint8_t));
    fread(bmp->body, 1, bmp-> size, bmp_file);
    
    //set current byte pointer to initial position
    bmp->current_byte = bmp->body;
    fclose(bmp_file);
    return bmp;
    
}

void free_params(Parameters * params){
    free(params->bmp->header);
    free(params->bmp);
    free(params->payload);
    free(params);
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot;
}