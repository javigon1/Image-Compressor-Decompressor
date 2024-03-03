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
                // printf("A: %ld,", cw->a);
                cw->b = (int64_t)(quantizeValues(averages.b, 0.3));
                // printf("[%d][%d]\n", col, row);
                // printf("After being quantized b: %ld,", cw->b);
                cw->c = (int64_t)(quantizeValues(averages.c, 0.3));
                // printf("c: %ld,", cw->c);
                cw->d = (int64_t)(quantizeValues(averages.d, 0.3));
                // printf("d: %ld\n******\n", cw->d);
                cw->pb = (uint64_t)(Arith40_index_of_chroma(averages.avg_pb));
                // printf("PB: %ld,", cw->pb);
                cw->pr = (uint64_t)(Arith40_index_of_chroma(averages.avg_pr));
                // printf("PR: %ld\n", cw->pr);
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
        
        // printf("COL, ROW = [%d,%d]\n", col, row);

        struct closure *closure = (struct closure *)cl;

        // printf("Width: %d\n",closure->methods->width(UArray2));
        // printf("Height: %d\n",closure->methods->height(UArray2));

        code_word cw = elem;

        (void)UArray2;


        // printf("[%d][%d]\n", col, row);

        // // printf("[%d,%d] A: %lu,",col, row, cw->a);
        // printf("As the element in the mapping function b: %ld,", cw->b);
        // printf("c: %ld,", cw->c);
        // printf("d: %ld\n ******\n", cw->d);
        // printf("PB: %lu,", cw->pb);
        // printf("PR: %lu\n", cw->pr);

        Pnm_ypbpr ypbpr_pixel1 = (Pnm_ypbpr)closure->methods->at(closure->array, col, row);
        Pnm_ypbpr ypbpr_pixel2 = (Pnm_ypbpr)closure->methods->at(closure->array, col + 1, row);
        Pnm_ypbpr ypbpr_pixel3 = (Pnm_ypbpr)closure->methods->at(closure->array, col, row + 1);
        Pnm_ypbpr ypbpr_pixel4 = (Pnm_ypbpr)closure->methods->at(closure->array, col + 1, row + 1);

        // printf("value of a: %ld, b: %ld, c: %ld, d: %ld\n",cw->a, cw->b, cw->c, cw->d);

        float a = ((float)(cw->a) / 511);
        float b = ((float)(cw->b) / 50);
        float c = ((float)(cw->c) / 50);
        float d = ((float)(cw->d) / 50);

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