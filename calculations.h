#ifndef CALCULATIONS_INCLUDED
#define CALCULATIONS_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <inttypes.h>
#include "image.h"
#include "RGB_component.h"


typedef struct {
        float a;
        float b;
        float c;
        float d;
        float avg_pb;
        float avg_pr;
} ChromaAverages;


float roundValues(float value, float lower_range, float higher_range);
ChromaAverages computeAverageChromas(Pnm_ypbpr Cv_pixel1, Pnm_ypbpr Cv_pixel2, 
                              Pnm_ypbpr Cv_pixel3, Pnm_ypbpr Cv_pixel4);
int64_t quantizeValues(float value, float range_limits);


#endif