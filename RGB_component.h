#ifndef RGB_COMPONENT_INCLUDED
#define RGB_COMPONENT_INCLUDED

#include "image.h"

/* REPRESENTS THE PIXEL'S COMPONENT VIDEO */
typedef struct Pnm_ypbpr {
        float y;
        float pb;
        float pr;
} *Pnm_ypbpr;


A2Methods_UArray2 rgb_to_cv(Pnm_ppm image, A2Methods_mapfun *map, A2Methods_T methods);
void applyRGB(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl);
Pnm_ppm cv_to_rgb(A2Methods_UArray2 image, A2Methods_mapfun *map, A2Methods_T methods);
void applyCV(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl);



#endif
