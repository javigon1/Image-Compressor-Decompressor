#include "cv_dct.h"


#define MAXVAL 255

/*
 * cv_to_DCT
 * Description: Takes a UArray2 of component value structs and performs the
 *              discrete cosine transformation on the struct member values
 * Parameters:  A2Methods_UArray2 cv_image - the UArray2 of component values to
 *                                           convert into code_word structs
 *              A2Methods_mapfun *map - pointer to the default UArray2 mapping
 *                                      function
 *              A2Methods_T methods - UArray2 method suite
 * Return:      a UArray2 of width/2 x height/2 containing the code_word struct
 *              instances, each of whose values are created from a 2x2 block of
 *              component value struct instances
 * Expects:     cv_image to not be NULL
 * Notes:
 *
 */
A2Methods_UArray2 cv_to_DCT(A2Methods_UArray2 cv_image, A2Methods_mapfun *map, 
                            A2Methods_T methods)
{
        /* create a UArray2 of half the dimensions to store the code words*/
        A2Methods_UArray2 DCT_image = methods->new((methods->width(cv_image)) / 2, 
                                                   (methods->height(cv_image)) / 2,
                                                    sizeof(struct code_word));

        struct closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;
        cl->array = DCT_image;
        cl->denominator = MAXVAL;

        /* map the component video image while converting everything to dct */
        map(cv_image, applyDCT, cl);

        /* checks if the closure allocated is NULL and if not frees it */
        if(cl) {
                free(cl);
                cl = NULL;
        }

        return DCT_image;
}

/*
 * applyDCT
 * Description: apply function for UArray2 mapping, converts one 2x2 block of
 *              component value structs to a code_word struct in the new UArray2
 * Parameters:  int col, row - indices of the UArray2 of component value structs
 *                             to map to col/2, row/2 in the array of code_words
 *              A2Methods_UArray2 UArray2 - UArray2 of component value structs
 *              void *elem - pointer to the current component value struct
 *              void *cl - pointer to a closure containing the array of
 *                         code_words, the UArray2 method suite, and the Maxval
 *                         of the original ppm image
 * Return:      none
 * Expects:     col, row to be in range
 *              UArray2, elem, cl to not be NULL
 * Notes:
 *
 */
void applyDCT(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        struct closure *closure = (struct closure *)cl;

        /* only want to do this for the top left cell of each 2x2 block 
                        - only in even coordinates                   */
        if (col % 2 == 0 && row % 2 == 0 
            && col < closure->methods->width(UArray2) 
            && row < closure->methods->height(UArray2)) {

                /* the top left pixel of the block */
                Pnm_ypbpr ypbpr_pixel1 = elem;
                Pnm_ypbpr ypbpr_pixel2 = (Pnm_ypbpr)closure->methods->at(UArray2, col + 1, row);
                Pnm_ypbpr ypbpr_pixel3 = (Pnm_ypbpr)closure->methods->at(UArray2, col, row + 1);
                Pnm_ypbpr ypbpr_pixel4 = (Pnm_ypbpr)closure->methods->at(UArray2, col + 1, row + 1);

                /* compute the averages of all of the pixels */
                ChromaAverages averages = computeAverageChromas(ypbpr_pixel1,
                                                                ypbpr_pixel2, 
                                                                ypbpr_pixel3, 
                                                                ypbpr_pixel4);

                code_word cw = closure->methods->at(closure->array, col / 2, row / 2);

                /* quantize each value to make sure it fits under the expected
                regions, get the index of pb and pr with the function given */
                cw->a = (uint64_t)(round(averages.a * 511));
                cw->b = (int64_t)(quantizeValues(averages.b, 0.3));
                cw->c = (int64_t)(quantizeValues(averages.c, 0.3));
                cw->d = (int64_t)(quantizeValues(averages.d, 0.3));
                cw->pb = (uint64_t)(Arith40_index_of_chroma(averages.avg_pb));
                cw->pr = (uint64_t)(Arith40_index_of_chroma(averages.avg_pr));
        }
}

/*
 * DCT_to_cv
 * Description: Takes a UArray2 of code_word structs and performs the inverse
 *              discrete cosine transformation on the member values
 * Parameters:  A2Methods_UArray2 DCT_image - the UArray2 of code_word structs
 *                                            to convert into component values
 *              A2Methods_mapfun *map - pointer to the default UArray2 mapping
 *                                      function
 *              A2Methods_T methods - UArray2 method suite
 * Return:      a width x height UArray2 containing the component value struct
 *              instances, each 2x2 block of which are created from a single
 *              code_word struct instance
 * Expects:     DCT_image to not be NULL
 * Notes:
 *
 */
A2Methods_UArray2 DCT_to_cv(A2Methods_UArray2 DCT_image, A2Methods_mapfun *map, 
                            A2Methods_T methods)
{
        /* have to create a UArray2 with double the dimension to decompress */
        int width = (methods->width(DCT_image)) * 2;
        int height = (methods->height(DCT_image)) * 2;

        A2Methods_UArray2 cv_image = methods->new(width, 
                                                  height,
                                                  sizeof(struct Pnm_ypbpr));

        struct closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;
        cl->array = cv_image;

        /* maps the UArray2 while converting back each codeword into ypbpr */
        map(DCT_image, applyCv, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return cv_image;
}

/*
 * applyCv
 * Description: apply function for UArray2 mapping, converts a code_word struct
 *              to a 2x2 block of component value structs in the new UArray2
 * Parameters:  int col, row - indices of the UArray2 of code_words to map to
 *                             2*col, 2*row in the array of code_words
 *              A2Methods_UArray2 UArray2 - UArray2 of component value structs
 *              void *elem - pointer to the current component value struct
 *              void *cl - pointer to closure containing the array of component
 *                         values, the UArray2 method suite, and the Maxval
 *                         of the original ppm image
 * Return:      none
 * Expects:     col, row to be in range
 *              UArray2, elem, cl to not be NULL
 * Notes:
 *
 */
void applyCv(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;

        struct closure *closure = (struct closure *)cl;
        code_word cw = elem;

        /* have to map to the top left cell of the new array so multiple the
        values of the coordinates by 2 */
        col *= 2;
        row *= 2;

        Pnm_ypbpr ypbpr_pixel1 = (Pnm_ypbpr)closure->methods->at(closure->array, col, row);
        Pnm_ypbpr ypbpr_pixel2 = (Pnm_ypbpr)closure->methods->at(closure->array, col + 1, row);
        Pnm_ypbpr ypbpr_pixel3 = (Pnm_ypbpr)closure->methods->at(closure->array, col, row + 1);
        Pnm_ypbpr ypbpr_pixel4 = (Pnm_ypbpr)closure->methods->at(closure->array, col + 1, row + 1);

        /* reverse the quantization given in the opposite conversion */
        float a = ((float)(cw->a) / 511);
        /* 50 = 10/3 * 15 */
        float b = ((float)(cw->b) / 50);
        float c = ((float)(cw->c) / 50);
        float d = ((float)(cw->d) / 50);

        /* arithmetic given */
        ypbpr_pixel1->y = a - b - c + d;
        ypbpr_pixel2->y = a - b + c + d;
        ypbpr_pixel3->y = a + b - c + d;
        ypbpr_pixel4->y = a + b + c + d;

        ypbpr_pixel1->pb = Arith40_chroma_of_index(cw->pb);
        ypbpr_pixel2->pb = Arith40_chroma_of_index(cw->pb);
        ypbpr_pixel3->pb = Arith40_chroma_of_index(cw->pb);
        ypbpr_pixel4->pb = Arith40_chroma_of_index(cw->pb);

        ypbpr_pixel1->pr = Arith40_chroma_of_index(cw->pr);
        ypbpr_pixel2->pr = Arith40_chroma_of_index(cw->pr);
        ypbpr_pixel3->pr = Arith40_chroma_of_index(cw->pr);
        ypbpr_pixel4->pr = Arith40_chroma_of_index(cw->pr);
}
