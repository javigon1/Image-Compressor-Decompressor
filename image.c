#include "image.h"


#define INDEX0 0
#define INDEX1 8
#define INDEX2 16
#define INDEX3 24

#define BYTE_WIDTH 8

#define DENOM 255


Except_T Smaller_Than_2x2 = {"Image can't be compressed - smaller than 2x2"};


Pnm_ppm readImagePpm(FILE *fp, A2Methods_T methods) 
{
        Pnm_ppm image = Pnm_ppmread(fp, methods);

        if (image->width < 2 || image->height < 2) {
                Pnm_ppmfree(&image);
                RAISE(Smaller_Than_2x2);
                exit(EXIT_FAILURE);  
        }

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
                methods->free(&(image->pixels));
                image->pixels = new_image;
        }
        
        return image;
}       


void printCompressedImage(A2Methods_UArray2 codeword_array, A2Methods_mapfun *map, 
                          A2Methods_T methods)
{
        (void)map;
        int width = (methods->width(codeword_array)) * 2;
        int height = (methods->height(codeword_array)) * 2;

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

        methods->free(&codeword_sequence);    
}

void applyPrintWord(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;
        (void)col;
        (void)row;
        (void)cl;

        uint64_t current_word = *(uint64_t*)elem;

        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX3));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX2));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX1));
        putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX0));
}


A2Methods_UArray2 readCompressedImage(FILE *fp, A2Methods_T methods)
{
        unsigned height, width;
        int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", &width, &height);
        assert(read == 2);
        int c = getc(fp);
        assert(c == '\n');

        width = width / 2;
        height = height / 2;

        A2Methods_UArray2 image = methods->new(width, 
                                               height,
                                               sizeof(uint64_t));

        for (unsigned i = 0; i < height; i++) {
                for (unsigned j = 0; j < width; j++){
                        uint64_t codeword = 0;
                        for (int index = 3; index >= 0; index--) {
                                uint64_t byte = getc(fp);
                                codeword = Bitpack_newu(codeword, 8, (index * 8), byte);
                        }
                        *(uint64_t *)methods->at(image, j, i) = codeword;
                }
        }

        return image;
}