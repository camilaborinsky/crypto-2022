#include <unistd.h>
#include <getopt.h>
#include <stegobmp.h>
#include <hide.h>
#include <reveal.h>
#include <structs.h>



int main(int argc, char *argv[])
{
    // Read arguments
    printf("%d\n", argc);
    Parameters params = parse_arguments(argc, argv);

    int return_value = 0;

    if (params.hide == 1)
        return_value = hide(params);
    else
        return_value = reveal(params);
    
    return return_value;
}

Parameters parse_arguments(int argc, char *argv[]){
    Parameters params;
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
                params.hide = 0;
                break;

            case 'e':
                params.hide = 1;
                break;

            case 'p':
                // params.bmp = optarg;
                break;

            case 'o':
                params.out_file = optarg;
                break;

            case 'i':
                // params.payload = optarg;
                break;

            case 's':
                params.steg = optarg;
                break;

            case 'a':
                params.enc_alg = optarg;
                break;

            case 'm':
                params.enc_mode = optarg;
                break;

            case 'k':
                params.password = optarg;
                break;
            default:
                abort ();
            }
        }
    return params;
        
    
}
