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

/*
 * packWords
 * Description: packs the member values of a code_word struct into a 32-bit
 *              uint64_t
 * Parameters:  A2Methods_UArray2 DCT_image - UArray2 of code_word struct
 *                                            instances
 *              A2Methods_mapfun *map - pointer to default mapping function
 *              A2Methods_T methods - UArray2 method suite
 * Return:      UArray2 of 32-bit codewords
 * Expects:     DCT_image to not be NULL
 * Notes:
 *
 */
A2Methods_UArray2 packWords(A2Methods_UArray2 DCT_image, A2Methods_mapfun *map, 
                            A2Methods_T methods)
{
        int width = methods->width(DCT_image);
        int height = methods->height(DCT_image);

        /* created a new UAray2 to store the uint64_t's */
        A2Methods_UArray2 codeword_array = methods->new(width, 
                                                        height,
                                                        sizeof(uint64_t));

        struct closure *cl = malloc(sizeof(*cl));

        cl->methods = methods;
        cl->array = codeword_array;

        /* pack each word */
        map(DCT_image, applyPack, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return codeword_array;
}

/*
 * applyPack
 * Description: packs the member values of a code_word struct into a 32-bit
 *              uint64_t
 * Parameters:  int col, int row - current index in code_word struct array
 *              A2Methods_UArray2 UArray2 - unused
 *              void *elem - pointer to the current code_word struct instance
 *              void *cl - closure containing the UArray2 to hold the 32-bit
 *                         uint64_t codewords, and the UArray2 method suite
 * Return:      none
 * Expects:     col, row to be in range
 *              *cl, *elem to not be NULL
 * Notes:
 *
 */
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
}

/*
 * unpackWords
 * Description: unpacks the member values of a code_word struct from a 32-bit
 *              uint64_t
 * Parameters:  A2Methods_UArray2 codeword_array - UArray2 of 32-bit codewords
 *              A2Methods_mapfun *map - pointer to default mapping function
 *              A2Methods_T methods - UArray2 method suite
 * Return:      UArray2 of code_word struct instances
 * Expects:     codeword_array to not be NULL
 * Notes:
 *
 */
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

/*
 * applyUnpack
 * Description: unpacks a single 32-bit codeword into a code_word struct
 * Parameters:  int col, int row - current index in 32-bit codeword array
 *              A2Methods_UArray2 UArray2 - unused
 *              void *elem - pointer to the current 32-bit codeword
 *              void *cl - closure containing the UArray2 to hold the code_word
 *                         struct, and the UArray2 method suite
 * Return:      none
 * Expects:     col, row to be in range
 *              *cl, *elem to not be NULL
 * Notes:
 *
 */
void applyUnpack(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;
        struct closure *closure = (struct closure *)cl;

        uint64_t *word = (uint64_t*)elem;
        code_word cw = closure->methods->at(closure->array, col, row);
        
        cw->a = Bitpack_getu(*word, WIDTH_A, LSB_A);
        cw->b = Bitpack_gets(*word, WIDTH_B, LSB_B);
        cw->c = Bitpack_gets(*word, WIDTH_C, LSB_C);
        cw->d = Bitpack_gets(*word, WIDTH_D, LSB_D);
        cw->pb = Bitpack_getu(*word, WIDTH_PB, LSB_PB);
        cw->pr = Bitpack_getu(*word, WIDTH_PR, LSB_PR);
}        