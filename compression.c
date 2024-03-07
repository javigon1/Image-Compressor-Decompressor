#include "compression.h"


#define A2M A2Methods_UArray2

/*
 * compress40
 * Description: Takes a file pointer and compresses its contents given that it
 *              points to a .ppm image
 * Parameters:  FILE *input - the file pointer containing the image to compress
 * Return:      none
 * Expects:     FILE to not be NULL
 * Notes:       results in a cre if methods or map is NULL
 *
 */
void compress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm image = readImagePpm(input, methods);
        assert(image);

        /* convert the image from into a UArray2 of Pnm_ypbpr structs */
        A2M cv_image = rgb_to_cv(image, map, methods);

        /* converts each 2x2 block into a code word struct */
        A2M wordImage = cv_to_DCT(cv_image, map, methods);

        /* packs each code word struct into a 32 bit word */
        A2M codeword_array = packWords(wordImage, map, methods);
        
        /* prints the header and the compressed image to stdout */
        printCompressedImage(codeword_array, map, methods);
        
        /* frees all of the images */
        Pnm_ppmfree(&image); 
        methods->free(&cv_image);
        methods->free(&wordImage);
        methods->free(&codeword_array);
}

/*
 * decompress40
 * Description: Takes a file pointer and decompresses and prints its contents
 *              given that it points to a compressed .ppm image
 * Parameters:  FILE *input - file pointer containing the image to decompress
 * Return:      none
 * Expects:     FILE to not be NULL
 * Notes:       results in a cre if methods or map is NULL
 *
 */
void decompress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        /* read in the information and the image compressed information */
        A2M information = readCompressedImage(input, methods);

        /* for each 32 bit word, we unpack it into a code word struct */
        A2M codeword_array = unpackWords(information, map, methods);

        /* converts each code word into a Pnm_ypbpr structs */
        A2M cv_image = DCT_to_cv(codeword_array, map, methods);

        /* converts the Pnm_ypbpr structs into their respective Pnm_rgb pix */
        Pnm_ppm pixmap = cv_to_rgb(cv_image, map, methods);

        /* print the image to stdout */
        Pnm_ppmwrite(stdout, pixmap);

        /* frees all of the images */
        methods->free(&information);
        methods->free(&codeword_array);
        methods->free(&cv_image);
        Pnm_ppmfree(&pixmap);
}
