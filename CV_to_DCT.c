#include "CV_to_DCT.h"


#define MAXVAL 255


A2Methods_UArray2 cv_to_DCT(A2Methods_UArray2 image, A2Methods_mapfun *map, 
                            A2Methods_T methods)
{
        A2Methods_UArray2 DCT_image = methods->new((methods->width(image)) / 2, 
                                                  (methods->height(image)) / 2,
                                                  sizeof(struct code_word));
        struct closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;
        cl->array = DCT_image;
        cl->denominator = MAXVAL;

        map(DCT_image, applyDCT, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return DCT_image;
}


void applyDCT(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl) 
{
        (void)UArray2;

        if (col % 2 == 0 && row % 2 == 0) {
                struct closure *closure = (struct closure *)cl;

                Pnm_ypbpr ypbpr_pixel1 = elem;
                // Pnm_ypbpr ypbpr_pixel1 = (Pnm_ypbpr)closure->methods->at(UArray2, col, row);
                Pnm_ypbpr ypbpr_pixel2 = (Pnm_ypbpr)closure->methods->at(UArray2, col + 1, row);
                Pnm_ypbpr ypbpr_pixel3 = (Pnm_ypbpr)closure->methods->at(UArray2, col, row + 1);
                Pnm_ypbpr ypbpr_pixel4 = (Pnm_ypbpr)closure->methods->at(UArray2, col + 1, row + 1);
                ChromaAverages averages = computeAverageChromas(ypbpr_pixel1,
                                                                ypbpr_pixel2, 
                                                                ypbpr_pixel3, 
                                                                ypbpr_pixel4);

                code_word cw = closure->methods->at(closure->array, col / 2, row / 2);
                /* IS THIS THE CORRECT ROUNDING? SPEC 8 */
                cw->a = (uint64_t)(round(averages.a * 511));
                cw->b = quantizeValues(averages.b, 0.3);
                cw->c = quantizeValues(averages.c, 0.3);
                cw->d = quantizeValues(averages.d, 0.3);
                cw->pb = (uint64_t)(Arith40_index_of_chroma(averages.avg_pb));
                cw->pr = (uint64_t)(Arith40_index_of_chroma(averages.avg_pr));
        } 

}