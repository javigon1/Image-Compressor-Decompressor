#include "image.h"


#define INDEX0 0
#define INDEX1 8
#define INDEX2 16
#define INDEX3 24

#define BYTE_WIDTH 8

#define DENOM 255


Except_T Smaller_Than_2x2 = {"Image can't be compressed - smaller than 2x2"};

/*
 * readImagePpm
 * Description: reads a .ppm image, trims the dimensions so that they are even,
 *              and populates a UArray2 with the red, blue, and green data of
 *              each pixel in the image
 * Parameters:  FILE *fp - pointer to the file containing the image
 *              A2Methods_T methods - UArray2 method suite
 * Return:      a Ppm_pnm struct instance containing a UArray2 with the pixel
 *              data of the image
 * Expects:     *fp to not be NULL
 * Notes:
 *
 */
Pnm_ppm readImagePpm(FILE *fp, A2Methods_T methods) 
{
        Pnm_ppm image = Pnm_ppmread(fp, methods);

        /* check if the dimensions of the image are at least 2x2 */
        if (image->width < 2 || image->height < 2) {
                Pnm_ppmfree(&image);
                RAISE(Smaller_Than_2x2);
                exit(EXIT_FAILURE);  
        }

        /* check if one of the dimensions is odd and needs to be trimmed */
        bool oddDimensions = false;
        A2Methods_UArray2 new_image;

        if (image->width % 2 != 0)
        {
                (image->width)--;
                oddDimensions = true;
        }

        if (image->height % 2 != 0) {
                (image->height)--;
                oddDimensions = true;
        }

        /* if one of the dimensions is odd then we have to create a new array
        and populate it with the same elements */
        if (oddDimensions) {
                new_image = methods->new(image->width, 
                                         image->height,
                                         sizeof(struct Pnm_rgb));
                for (int i = 0; i < methods->width(new_image); i++) {
                        for (int j = 0; j < methods->height(new_image); j++) {
                                Pnm_rgb pixel = methods->at(image->pixels, i, j);
                                Pnm_rgb new_pixel = methods->at(new_image, i, j);
                                new_pixel->red = pixel->red;
                                new_pixel->blue = pixel->blue;
                                new_pixel->green = pixel->green;
                        }
                }
                /* free the old array and assign the new one to image */
                methods->free(&(image->pixels));
                image->pixels = new_image;
        }
        
        return image;
}

/*
 * printCompressedImage
 * Description: prints a compressed image to stdout
 * Parameters:  A2Methods_UArray2 codeword_array - UArray2 of 32-bit codewords
 *              A2Methods_mapfun *map - UArray2 default mapping function
 *              A2Methods_T methods - UArray2 method suite
 * Return:      none
 * Expects:     codeword_array to not be null
 * Notes:
 *
 */
void printCompressedImage(A2Methods_UArray2 codeword_array, A2Methods_mapfun *map, 
                          A2Methods_T methods)
{
        /* the original dimensions are double the size of the old one */
        int width = (methods->width(codeword_array)) * 2;
        int height = (methods->height(codeword_array)) * 2;

        /* print the header information to stdout */
        printf("COMP40 Compressed image format 2\n%u %u\n", width, height);

        A2Methods_UArray2 codeword_sequence = methods->new(width, 
                                                           height,
                                                           sizeof(uint64_t));

        struct closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;
        cl->array = codeword_sequence;

        /* prints each word we read from the UArray2 */
        map(codeword_array, applyPrintWord, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }    

        methods->free(&codeword_sequence);    
}

/*
 * applyPrintWord
 * Description: prints a single 32-bit code word to stdout in Big-endian order
 * Parameters:  int col, int row - unused
 *              A2Methods_UArray2 UArray2 - unused
 *              void *elem - pointer to the current 32-bit codeword to print
 *              void *cl - unused
 * Return:      none
 * Expects:     codeword_array to not be null
 * Notes:
 *
 */
void applyPrintWord(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;
        (void)col;
        (void)row;
        (void)cl;

        uint64_t current_word = *(uint64_t*)elem;

        /* print the information in reverse order to satisfy the Big Endian */
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX3));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX2));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX1));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX0));
}

/*
 * readCompressedImage
 * Description: reads a compressed image and puts the data of the 32-bit
 *              code words into a UArray2
 * Parameters:  FILE *fp - pointer to the file containing the compressed image
 *              A2Methods_T methods - UArray2 method suite
 * Return:      a UArray2 with the data of the image in Big-endian order
 * Expects:     *fp to not be NULL
 * Notes:
 *
 */
A2Methods_UArray2 readCompressedImage(FILE *fp, A2Methods_T methods)
{
        /* read in the header and store the dimensions of the original image */
        unsigned height, width;
        int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", &width, &height);
        assert(read == 2);
        int c = getc(fp);
        assert(c == '\n');

        /* we know that the height and the width of the intial array will be 
        half of those of the original */
        width = width / 2;
        height = height / 2;

        A2Methods_UArray2 image = methods->new(width, 
                                               height,
                                               sizeof(uint64_t));

        /* maps the UArray2 in row major mapping and for each element we store
        in a word in multiples of 4 (32 bit wors and getc gets 1 byte) */
        for (unsigned i = 0; i < height; i++) {
                for (unsigned j = 0; j < width; j++){
                        uint64_t codeword = 0;
                        for (int index = 3; index >= 0; index--) {
                                uint64_t byte = getc(fp);
                                codeword = Bitpack_newu(codeword, 8, (index * 8), byte);
                        }
                        /* map those values to the image we created*/
                        *(uint64_t *)methods->at(image, j, i) = codeword;
                }
        }

        return image;
}