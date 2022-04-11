#include "hide.h"
#include "structs.h"

int hide(Parameters params){
    // Check that bmp file is indeed bmp looking at extension. Check no double extension (.bmp.txt)
    // Using tokenize probably


    // Check that bmp file is long enough for the payload to fit. If not throw error

    // Encrypt using openssl if encryption was requested

    // If encrpytion was requested, check that bmp file is long enough for new payload to fit

    // steganograph: file_size || file || ".extension\0"

    // Hide the data

    return 0;
}


void hide_lsb1(){
    // on each block replace least significant bit with message bit

}

void hide_lsb4(){
    // on each block replace least 4 significant bits with 4 message bits


}

void hide_lsbi(){
    
}