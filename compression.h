#ifndef COMPRESSION_INCLUDED
#define COMPRESSION_INCLUDED

#include "image.h"
#include "arith40.h"
#include "cv_dct.h"
#include "packUnpack.h"
#include "rgb_cv.h"
#include "except.h"


void compress40(FILE *input);
void decompress40(FILE *input);


#endif