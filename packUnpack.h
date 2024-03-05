#ifndef PACKUNPACK_INCLUDED
#define PACKUNPACK_INCLUDED

#include "calculations.h"
#include "RGB_component.h"
#include "image.h"
#include "bitpack.h"
#include "CV_to_DCT.h"


A2Methods_UArray2 packWords(A2Methods_UArray2 DCT_image, A2Methods_mapfun *map, 
                            A2Methods_T methods);
void applyPack(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl);
A2Methods_UArray2 unpackWords(A2Methods_UArray2 codeword_array, A2Methods_mapfun *map, 
                              A2Methods_T methods);
void applyUnpack(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl);




#endif