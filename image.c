#include "image.h"
#include "bitpack.h"


#define INDEX0 0
#define INDEX1 8
#define INDEX2 16
#define INDEX3 24

#define BYTE_WIDTH 8


#define A2M A2Methods_UArray2


Pnm_ppm readImagePpm(FILE *fp, A2Methods_T methods) 
{
        Pnm_ppm image = Pnm_ppmread(fp, methods);

        bool oddDimensions = false;
        A2M new_image;

        if (image->width % 2 != 0) {
                (image->width)--;
                oddDimensions = true;
        }

        if (image->height % 2 != 0) {
                (image->height)--;
                oddDimensions = true;
        }

        if (oddDimensions == false) {
                return image;
        } else {
                new_image = methods->new(methods->width(image), 
                                              methods->height(image),
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
        }

        image->pixels = new_image;
        methods->free(&(image->pixels));
        Pnm_ppmfree(&image);
        
        return new_image;
}       


void printCompressedImage(A2Methods_UArray2 codeword_array, A2Methods_mapfun *map, 
                          A2Methods_T methods)
{
        (void)map;
        int width = methods->width(codeword_array);
        int height = methods->height(codeword_array);

        printf("COMP40 Compressed image format 2\n%u %u\n", width, height);

        A2Methods_UArray2 codeword_sequence = methods->new(width, 
                                                           height,
                                                           sizeof(uint64_t));

        struct closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;
        cl->array = codeword_sequence;

        map(codeword_array, applyPrintWord, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }        
}

void applyPrintWord(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;
        (void)col;
        (void)row;
        (void)cl;
        // struct closure *closure = (struct closure *)cl;

        uint64_t current_word = (uint64_t)elem;

        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX0));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX1));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX2));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX3));
}



// void printImagePpm(A2Methods_UArray2 codeword_array, A2Methods_T methods)
// {

// }