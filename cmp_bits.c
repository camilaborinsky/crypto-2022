#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

size_t count_left(FILE * file){
    size_t count = 0;
    uint8_t buf[2] = {0,0};
    while( fread(buf , 1 , 1, file) !=0){
        count++;
    }
    return count*8;
}

size_t cmp_bits(FILE * file1 , FILE * file2, int final_flag){

    uint8_t byte1 = 0;//read
    uint8_t byte2 = 0;//read
    size_t diff = 0;

    uint8_t buf1[3] = {0,0,0};
    uint8_t buf2[3] = {0,0,0};

    size_t error1 = 0;
    size_t error2 = 0;

    error1 = fread(buf1 ,1 ,1 ,file1);
    error2 = fread(buf2 ,1 ,1 ,file2);

    for(int j = 0;  error1 != 0 && error2 != 0 ; j++){

        byte1 = buf1[0];
        byte2 = buf2[0];

        for(int i=0 ;i<8; i++ ){

            uint8_t mask = 0x1 << (i);
            uint8_t masked_byte_1 = byte1 & mask;
            uint8_t masked_byte_2 = byte2 & mask; 
            if(masked_byte_1 != masked_byte_2){
                diff++;
            }
        }

        error1 = fread(buf1 ,1 ,1 ,file1 );
        error2 = fread(buf2 ,1 ,1 ,file2 );

    }

    if(!final_flag){
        return diff;
    }

    if(error1 != 0 ){
        printf("file1 is larger than file2\n");
        diff+= count_left(file1);
    }
    if(error2 != 0){
        printf("file2 is larger than file1\n");
        diff+= count_left(file2);
    }
    return diff;
}

int main(int argc, char *argv[]){
    if(argv[1] == NULL || argv[2]==NULL){
        printf("Se deben pasar dos archivos como parametro\n");
    }
    FILE * file1 = fopen(argv[1] , "r");
    FILE * file2 = fopen(argv[2] , "r");
    if(file1 == NULL){
        printf("Error abriendo el archivo 1\n");
    }
    if(file2 == NULL){
        printf("Error abriendo el archivo 2\n");
    }
    if(file1 == NULL || file2 == NULL){
        return -1;
    }
    size_t diff = cmp_bits(file1 , file2 , 1);
    printf("There are %ld bits of difference\n" , diff );
    return 1;
}