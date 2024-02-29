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


Pnm_ppm readImagePpm(FILE *fp, A2Methods_T methods);


#endif
