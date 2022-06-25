
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
        diff+= count_left(file1);
    }
    if(error2 != 0){
        diff+= count_left(file2);
    }
    return diff;
}