#include "image.h"
#include "bitpack.h"


#define INDEX0 0
#define INDEX1 8
#define INDEX2 16
#define INDEX3 24

#define BYTE_WIDTH 8

#define DENOM 255


#define A2M A2Methods_UArray2


Pnm_ppm readImagePpm(FILE *fp, A2Methods_T methods) 
{
        Pnm_ppm image = Pnm_ppmread(fp, methods);

        bool oddDimensions = false;
        A2M new_image;

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


       
        
        // Pnm_ppmfree(&image);
        
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

        // fprintf(stderr, "*[0][0] = %lu \n", *(uint64_t*)methods->at(codeword_array, 0, 0));
        // fprintf(stderr, "*[1][0] = %lu \n", *(uint64_t*)methods->at(codeword_array, 1, 0));

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
        // struct closure *closure = (struct closure *)cl;

        uint64_t current_word = *(uint64_t*)elem;
        
        // uint64_t x = Bitpack_getu(current_word, BYTE_WIDTH, INDEX3);
        // print_uint64_binary(x);
        // uint64_t x1 = Bitpack_getu(current_word, BYTE_WIDTH, INDEX2);
        // print_uint64_binary(x1);
        // uint64_t x2 = Bitpack_getu(current_word, BYTE_WIDTH, INDEX1);
        // print_uint64_binary(x2);
        // uint64_t x3 = Bitpack_getu(current_word, BYTE_WIDTH, INDEX0);
        // print_uint64_binary(x3);

        fprintf(stderr, "---[%d][%d]\n", col, row);
        print_uint64_binary(current_word);

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

        /* CHECK FOR THE SYNTAX IN THIS, WANT TO UPDATE THE IMAGE ARRAY WITH 
        CODE WORDS */
        // readCompressedWords(image, map, methods)
        for (unsigned i = 0; i < height; i++) {
                for (unsigned j = 0; j < width; j++){
                        /********************************************/
                        uint64_t codeword = 0;
                        for (int k = 3; k >= 0; k--) {
                                uint64_t byte = getc(fp);
                                fprintf(stderr, "before:  ");
                                print_uint64_binary(codeword);
                                codeword = Bitpack_newu(codeword, 8, (k * 8), byte);
                                fprintf(stderr, "after:   ");
                                print_uint64_binary(codeword);
                        }

                        uint64_t* x = (uint64_t *)methods->at(image, j, i) ;
                        *x = codeword;
                        /********************************************/
                        // fprintf(stderr, "index[%u][%u]...\n", i, j);
                        // uint64_t byte1 = (uint64_t)getc(fp);
                        // uint64_t byte2 = (uint64_t)getc(fp);
                        // uint64_t byte3 = (uint64_t)getc(fp);
                        // uint64_t byte4 = (uint64_t)getc(fp);

                        // uint64_t temp = Bitpack_newu(byte1, BYTE_WIDTH,
                        //                         INDEX1, byte2);
                        // uint64_t temp2 = Bitpack_newu(temp, BYTE_WIDTH,
                        //                         INDEX2, byte3);
                        // uint64_t temp3 = Bitpack_newu(temp2, BYTE_WIDTH,
                        //                         INDEX3, byte4);

                        // uint64_t *word = (uint64_t *)methods->at(image, j, i);
                        // *word = temp3;
                        // fprintf(stderr, "Height %d\n", i);
                        // fprintf(stderr, "Width %d\n", j);
                }
        }

        return image;
}


// A2Methods_UArray2 readCompressedWords(A2Methods_UArray2 codeword_array, A2Methods_mapfun *map, 
//                                        A2Methods_T methods);
// {       
        
// }

// void applyGetWord(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
// {
//         (void)UArray2;
//         (void)col;
//         (void)row;
//         (void)cl;
//         // struct closure *closure = (struct closure *)cl;

//         uint64_t current_word = (uint64_t)elem;

//         putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX0));
//         putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX1));
//         putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX2));
//         putchar(Bitpack_getu(current_word, BYTE_WIDTH, INDEX3));
// }