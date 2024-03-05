#include "CV_to_DCT.h"


#define MAXVAL 255


A2Methods_UArray2 cv_to_DCT(A2Methods_UArray2 cv_image, A2Methods_mapfun *map, 
                            A2Methods_T methods)
{
        A2Methods_UArray2 DCT_image = methods->new((methods->width(cv_image)) / 2, 
                                                   (methods->height(cv_image)) / 2,
                                                    sizeof(struct code_word));
        struct closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;
        cl->array = DCT_image;
        cl->denominator = MAXVAL;

        map(cv_image, applyDCT, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return DCT_image;
}


void applyDCT(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        struct closure *closure = (struct closure *)cl;

        if (col % 2 == 0 && row % 2 == 0 
            && col < closure->methods->width(UArray2) 
            && row < closure->methods->height(UArray2)) {

                Pnm_ypbpr ypbpr_pixel1 = elem;
                Pnm_ypbpr ypbpr_pixel2 = (Pnm_ypbpr)closure->methods->at(UArray2, col + 1, row);
                Pnm_ypbpr ypbpr_pixel3 = (Pnm_ypbpr)closure->methods->at(UArray2, col, row + 1);
                Pnm_ypbpr ypbpr_pixel4 = (Pnm_ypbpr)closure->methods->at(UArray2, col + 1, row + 1);

                ChromaAverages averages = computeAverageChromas(ypbpr_pixel1,
                                                                ypbpr_pixel2, 
                                                                ypbpr_pixel3, 
                                                                ypbpr_pixel4);
                code_word cw = closure->methods->at(closure->array, col / 2, row / 2);

                cw->a = (uint64_t)(round(averages.a * 511));
                cw->b = (int64_t)(quantizeValues(averages.b, 0.3));
                cw->c = (int64_t)(quantizeValues(averages.c, 0.3));
                cw->d = (int64_t)(quantizeValues(averages.d, 0.3));
                cw->pb = (uint64_t)(Arith40_index_of_chroma(averages.avg_pb));
                cw->pr = (uint64_t)(Arith40_index_of_chroma(averages.avg_pr));
        }
}


A2Methods_UArray2 DCT_to_cv(A2Methods_UArray2 DCT_image, A2Methods_mapfun *map, 
                            A2Methods_T methods)
{
        int width = (methods->width(DCT_image)) * 2;
        int height = (methods->height(DCT_image)) * 2;

        A2Methods_UArray2 cv_image = methods->new(width, 
                                                 height,
                                                 sizeof(struct Pnm_ypbpr));

        struct closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;
        cl->array = cv_image;

        map(DCT_image, applyCv, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return cv_image;
}


void applyCv(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        struct closure *closure = (struct closure *)cl;

        code_word cw = elem;

        (void)UArray2;

        Pnm_ypbpr ypbpr_pixel1 = (Pnm_ypbpr)closure->methods->at(closure->array, col, row);
        Pnm_ypbpr ypbpr_pixel2 = (Pnm_ypbpr)closure->methods->at(closure->array, col + 1, row);
        Pnm_ypbpr ypbpr_pixel3 = (Pnm_ypbpr)closure->methods->at(closure->array, col, row + 1);
        Pnm_ypbpr ypbpr_pixel4 = (Pnm_ypbpr)closure->methods->at(closure->array, col + 1, row + 1);

        /* CHECK FOR THESE VALUES FOR DIVISION */
        float a = ((float)(cw->a) / 511);
        float b = ((float)(cw->b));
        float c = ((float)(cw->c));
        float d = ((float)(cw->d));

        ypbpr_pixel1->y = a - b - c + d;
        ypbpr_pixel2->y = a - b + c + d;
        ypbpr_pixel3->y = a + b - c + d;
        ypbpr_pixel4->y = a + b + c + d;

        ypbpr_pixel1->pb = Arith40_chroma_of_index(cw->pb);
        ypbpr_pixel2->pb = Arith40_chroma_of_index(cw->pb);
        ypbpr_pixel3->pb = Arith40_chroma_of_index(cw->pb);
        ypbpr_pixel3->pb = Arith40_chroma_of_index(cw->pb);

        ypbpr_pixel1->pr = Arith40_chroma_of_index(cw->pr);
        ypbpr_pixel2->pr = Arith40_chroma_of_index(cw->pr);
        ypbpr_pixel3->pr = Arith40_chroma_of_index(cw->pr);
        ypbpr_pixel4->pr = Arith40_chroma_of_index(cw->pr);
}