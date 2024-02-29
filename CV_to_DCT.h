#ifndef CV_TO_DCT_INCLUDE
#define CV_TO_DCT_INCLUDE


#include "calculations.h"
#include "RGB_component.h"
#include "image.h"


/* REPRESENTS THE 32bit PACKED WORD */
typedef struct code_word {
        uint64_t a;
        int64_t  b;
        int64_t  c;
        int64_t  d;
        uint64_t pb;
        uint64_t pr;    
} *code_word;


A2Methods_UArray2 cv_to_DCT(A2Methods_UArray2 image, A2Methods_mapfun *map, 
                            A2Methods_T methods);
void applyDCT(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl);





#endif