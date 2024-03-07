#include "packUnpack.h"


#define LSB_A 23
#define LSB_B 18
#define LSB_C 13
#define LSB_D 8
#define LSB_PB 4
#define LSB_PR 0


#define WIDTH_A 9
#define WIDTH_B 5
#define WIDTH_C 5
#define WIDTH_D 5
#define WIDTH_PB 4
#define WIDTH_PR 4


A2Methods_UArray2 packWords(A2Methods_UArray2 DCT_image, A2Methods_mapfun *map, 
                            A2Methods_T methods)
{
        int width = methods->width(DCT_image);
        int height = methods->height(DCT_image);


        A2Methods_UArray2 codeword_array = methods->new(width, 
                                                        height,
                                                        sizeof(uint64_t));

        struct closure *cl = malloc(sizeof(*cl));

        cl->methods = methods;
        cl->array = codeword_array;

        map(DCT_image, applyPack, cl);

        fprintf(stderr, "*[0][0] = %lu \n", *(uint64_t*)methods->at(codeword_array, 0, 0));
        fprintf(stderr, "*[1][0] = %lu \n", *(uint64_t*)methods->at(codeword_array, 1, 0));
        
        if(cl) {
                free(cl);
                cl = NULL;
        }

        return codeword_array;

        // 00110010000111010100011101110100
        // 00110010000111010100011101110100
}


void applyPack(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;
        struct closure *closure = (struct closure *)cl;

        code_word codeword = elem;
        uint64_t *new_word = closure->methods->at(closure->array, col, row);

        *new_word = Bitpack_newu(*new_word, WIDTH_A, LSB_A, codeword->a);
        *new_word = Bitpack_news(*new_word, WIDTH_B, LSB_B, codeword->b);
        *new_word = Bitpack_news(*new_word, WIDTH_C, LSB_C, codeword->c);
        *new_word = Bitpack_news(*new_word, WIDTH_D, LSB_D, codeword->d);
        *new_word = Bitpack_newu(*new_word, WIDTH_PB, LSB_PB, codeword->pb);
        *new_word = Bitpack_newu(*new_word, WIDTH_PR, LSB_PR, codeword->pr);

        if (col == 0 && row == 0) {
                fprintf(stderr, "APPLYPACK\n");
        }

        if (col == 0 || col == 1 || col == 2 || col == 3 || col == 4) {
                if (row == 0 || row == 1 || row == 2 || row == 3 || row == 4) {
                        fprintf(stderr, "[%d][%d]\n", col, row);
                        print_uint64_binary(*new_word);
                }
        }
        
}


A2Methods_UArray2 unpackWords(A2Methods_UArray2 codeword_array, A2Methods_mapfun *map, 
                              A2Methods_T methods)
{
        int width = methods->width(codeword_array);
        int height = methods->height(codeword_array);

        A2Methods_UArray2 DCT_image = methods->new(width, 
                                                   height,
                                                   sizeof(struct code_word));

        struct closure *cl = malloc(sizeof(*cl));

        cl->methods = methods;
        cl->array = DCT_image;

        map(codeword_array, applyUnpack, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return DCT_image;
}


void applyUnpack(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;
        struct closure *closure = (struct closure *)cl;

        uint64_t *word = (uint64_t*)elem;
        code_word cw = closure->methods->at(closure->array, col, row);
        
        cw->a = Bitpack_getu(*word, WIDTH_A, LSB_A);
        // fprintf(stderr, "A: %lu\n", cw->a);
        cw->b = Bitpack_gets(*word, WIDTH_B, LSB_B);
        // fprintf(stderr, "B: %ld\n", cw->b);
        cw->c = Bitpack_gets(*word, WIDTH_C, LSB_C);
        // fprintf(stderr, "C: %ld\n", cw->c);
        cw->d = Bitpack_gets(*word, WIDTH_D, LSB_D);
        // fprintf(stderr, "D: %ld\n", cw->d);
        cw->pb = Bitpack_getu(*word, WIDTH_PB, LSB_PB);
        // fprintf(stderr, "PB: %lu\n", cw->pb);
        cw->pr = Bitpack_getu(*word, WIDTH_PR, LSB_PR);
        // fprintf(stderr, "PR: %lu\n", cw->pr);
}        