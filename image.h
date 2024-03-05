#ifndef IMAGE__INCLUDED
#define IMAGE__INCLUDED


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include "arith40.h"
#include "pnm.h"
#include "mem.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "bitpack.h"
#include "assert.h"

/* CONTAINS THE INFORMATION WE'LL PASS AS A CLOSURE TO THE MAPPINGS */
struct closure {
        A2Methods_T methods;
        A2Methods_UArray2 *array;
        unsigned int denominator;
};


Pnm_ppm readImagePpm(FILE *fp, A2Methods_T methods);
void printCompressedImage(A2Methods_UArray2 array, A2Methods_mapfun *map,
                          A2Methods_T methods);
void applyPrintWord(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl);
struct Pnm_ppm readHeaderImage(FILE *fp, A2Methods_T methods);
void applyGetWord(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)


#endif
