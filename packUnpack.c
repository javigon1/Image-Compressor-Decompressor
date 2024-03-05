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

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return codeword_array;
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
        // (void)UArray2;

        // struct closure *closure = (struct closure *)cl;

        // code_word codeword = elem;
        // uint64_t new_word = 0;

        // uint64_t *new_word_p = closure->methods->at(closure->array, col, row);

        // new_word = Bitpack_newu(new_word, WIDTH_A, LSB_A, codeword->a);
        // new_word = Bitpack_news(new_word, WIDTH_B, LSB_B, codeword->b);
        // new_word = Bitpack_news(new_word, WIDTH_C, LSB_C, codeword->c);
        // new_word = Bitpack_news(new_word, WIDTH_D, LSB_D, codeword->d);
        // new_word = Bitpack_newu(new_word, WIDTH_PB, LSB_PB, codeword->pb);
        // new_word = Bitpack_newu(new_word, WIDTH_PR, LSB_PR, codeword->pr);

        // *new_word_p = new_word;
}


// A2Methods_UArray2 unpackWords(A2Methods_UArray2 codeword_array, A2Methods_mapfun *map, 
//                               A2Methods_T methods)
// {
//         int width = methods->width(codeword_array);
//         int height = methods->height(codeword_array);


//         A2Methods_UArray2 DCT_image = methods->new(width, 
//                                                    height,
//                                                    sizeof(struct code_word));


//         struct closure *cl = malloc(sizeof(*cl));

//         cl->methods = methods;
//         cl->array = DCT_image;

//         map(codeword_array, applyPack, cl);

//         if(cl) {
//                 free(cl);
//                 cl = NULL;
//         }

//         return DCT_image;
// }


// void applyUnpack(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
// {

// }