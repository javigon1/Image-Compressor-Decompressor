#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compression.h"
#include "image.h"

// static void (*compress_or_decompress)(FILE *input) = compress40;

int main(int argc, char *argv[])
{

        if (argc > 2) {
                fprintf(stderr, "Too many arguments provided");
                exit(1);
        }
        
        FILE* fp = fopen(argv[1], "rb");

        if (!fp) {
                fprintf(stderr, 
                        "One of the files provided couldn't be opened\n");
                exit(EXIT_FAILURE);
        }

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm image = readImagePpm(fp, methods);
        fclose(fp);
        assert(image);

        printf("Width: %d\n", image->width);
        printf("Height: %d\n", image->height);


/******************************************************************************/
        // int i;

        // for (i = 1; i < argc; i++) {
        //         if (strcmp(argv[i], "-c") == 0) {
        //                 compress_or_decompress = compress40;
        //         } else if (strcmp(argv[i], "-d") == 0) {
        //                 compress_or_decompress = decompress40;
        //         } else if (*argv[i] == '-') {
        //                 fprintf(stderr, "%s: unknown option '%s'\n",
        //                         argv[0], argv[i]);
        //                 exit(1);
        //         } else if (argc - i > 2) {
        //                 fprintf(stderr, "Usage: %s -d [filename]\n"
        //                         "       %s -c [filename]\n",
        //                         argv[0], argv[0]);
        //                 exit(1);
        //         } else {
        //                 break;
        //         }
        // }
        // assert(argc - i <= 1);    /* at most one file on command line */
        // if (i < argc) {
        //         FILE *fp = fopen(argv[i], "r");
        //         assert(fp != NULL);
        //         compress_or_decompress(fp);
        //         fclose(fp);
        // } else {
        //         compress_or_decompress(stdin);
        // }

/******************************************************************************/

        return EXIT_SUCCESS; 
}