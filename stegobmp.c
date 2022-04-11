#include <unistd.h>
#include <getopt.h>
#include "stegobmp.h"
#include "hide.h"
#include "reveal.h"
#include "structs.h"


struct option{
    char * name;
    char required_arg;
    char * arg;
};

static struct option options [] = {
    {"extract", 0, 0},
    {"embed", 0, 0},
    {"steg", 1, 0}
};

int main(int argc, char const *argv[])
{
    // Read arguments
    Parameters params = parse_arguments(argc, argv);

    int return_value = 0;

    if (params.hide == 1)
        return_value = hide(params);
    else
        return_value = reveal(params);
    
    return return_value;
}

Parameters parse_arguments(int argc, char const *argv[]){
    char * op_str;
    char * op_arg;

    
    for(int i = 0; i < argc ; i++){
        op_str = argv[i];
    }
        
    
}
