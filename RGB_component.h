#ifndef RGB_COMPONENT_INCLUDED
#define RGB_COMPONENT_INCLUDED

#include "image.h"

/* REPRESENTS THE PIXEL'S COMPONENT VIDEO */
typedef struct Pnm_ypbpr {
        float y;
        float pb;
        float pr;
} *Pnm_ypbpr;

struct closure {
        A2Methods_T methods;
        A2Methods_UArray2 *array;
        unsigned int denominator;
};


float round_values(float value, float lower_range, float higher_range);
void applyRGB(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl);





#endif
