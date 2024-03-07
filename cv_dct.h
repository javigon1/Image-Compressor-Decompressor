#ifndef CV_DCT_INCLUDE
#define CV_DCT_INCLUDE


#include "calculations.h"
#include "rgb_cv.h"
#include "image.h"


/* represents the struct that is going to contain everything before packing it
into a 32 bit word, or after unpacking such word */
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
A2Methods_UArray2 DCT_to_cv(A2Methods_UArray2 DCT_image, A2Methods_mapfun *map,
                            A2Methods_T methods);
void applyCv(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl);

#endif