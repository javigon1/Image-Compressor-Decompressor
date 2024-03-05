#include "compression.h"
#include "arith40.h"
#include "image.h"
#include "CV_to_DCT.h"
#include "packUnpack.h"
#include "RGB_component.h"


void compress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm image = readImagePpm(input, methods);
        assert(image);

        A2Methods_UArray2 cv_image = rgb_to_cv(image, map, methods);

        A2Methods_UArray2 wordImage = cv_to_DCT(cv_image, map, methods);

        A2Methods_UArray2 codeword_array = packWords(wordImage, map, methods);
        
        printCompressedImage(codeword_array, map, methods);
        
        Pnm_ppmfree(&image); 
        methods->free(&cv_image);
        methods->free(&wordImage);
        methods->free(&codeword_array);
}



void decompress40(FILE *input)
{
        (void)input;
}
