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
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        A2Methods_UArray2 information = readCompressedImage(input, methods);

        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 4; j++) {
        //                 uint64_t num = (uint64_t)methods->at(information, j, i);
        //                 fprintf(stderr, "[%d][%d]", j, i);
        //                 print_uint64_binary(num);
        //         }
        // }

        A2Methods_UArray2 codeword_array = unpackWords(information, map, methods);

        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 4; j++) {
        //                 code_word word = (code_word)methods->at(codeword_array, j, i);
        //                 unsigned a = word->a;
        //                 signed b = word->b;
        //                 signed c = word->c;
        //                 signed d = word->d;
        //                 unsigned pb = word->pb;
        //                 unsigned pr = word->pr;
        //                 fprintf(stderr, "[%d][%d]\n", j, i);
        //                 fprintf(stderr, "a: %u, b: %d, c: %d, d: %d, pb: %u, pr: %u\n",
        //                         a, b, c, d, pb, pr);
        //         }
        // }

        A2Methods_UArray2 cv_image = DCT_to_cv(codeword_array, map, methods);

        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 4; j++) {
        //                 Pnm_ypbpr ypbpr_pixel = (Pnm_ypbpr)methods->at(cv_image, j, i);
        //                 float y = ypbpr_pixel->y;
        //                 float pb = ypbpr_pixel->pb;
        //                 float pr = ypbpr_pixel->pr;
        //                 fprintf(stderr, "[%d][%d]\n",j, i);
        //                 fprintf(stderr, "y: %f, pb: %f, pr: %f\n", y, pb, pr);
        //         }
        // }

        Pnm_ppm pixmap = cv_to_rgb(cv_image, map, methods);

        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 4; j++) {
        //                 Pnm_rgb first_pixel = (Pnm_rgb)methods->at(pixmap->pixels, j, i);
        //                 unsigned red = first_pixel->red;
        //                 unsigned green = first_pixel->green;
        //                 unsigned blue = first_pixel->blue;
        //                 fprintf(stderr, "[%d][%d]\n",j, i);
        //                 fprintf(stderr, "Red: %u, Green: %u, Blue: %u\n", red, green, blue);
        //         }
        // }

        Pnm_ppmwrite(stdout, pixmap);


        // methods->free(image.pixels); 
        // Pnm_ppmfree(&pixmap); 
        methods->free(&cv_image);
        methods->free(&codeword_array);
}
