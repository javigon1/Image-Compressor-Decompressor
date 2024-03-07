#include "compression.h"


#define A2M A2Methods_UArray2

void compress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm image = readImagePpm(input, methods);
        assert(image);

        A2M cv_image = rgb_to_cv(image, map, methods);

        A2M wordImage = cv_to_DCT(cv_image, map, methods);

        A2M codeword_array = packWords(wordImage, map, methods);
        
        printCompressedImage(codeword_array, map, methods);
        
        Pnm_ppmfree(&image); 
        methods->free(&cv_image);
        methods->free(&wordImage);
        methods->free(&codeword_array);
}



void decompress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        A2M information = readCompressedImage(input, methods);

        A2M codeword_array = unpackWords(information, map, methods);

        A2M cv_image = DCT_to_cv(codeword_array, map, methods);

        Pnm_ppm pixmap = cv_to_rgb(cv_image, map, methods);

        Pnm_ppmwrite(stdout, pixmap);

        methods->free(&cv_image);
        methods->free(&codeword_array);
}
