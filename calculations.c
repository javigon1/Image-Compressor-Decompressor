#include "calculations.h"
#include "RGB_component.h"
#include "CV_to_DCT.h"
#include "bitpack.h"

float roundValues(float value, float lower_range, float higher_range)
{
        if (value < lower_range) return lower_range;
        if (value > higher_range) return higher_range;
        return value;
}


ChromaAverages computeAverageChromas(Pnm_ypbpr Cv_pixel1, Pnm_ypbpr Cv_pixel2, 
                              Pnm_ypbpr Cv_pixel3, Pnm_ypbpr Cv_pixel4)
{
        ChromaAverages averages;
        averages.avg_pb = (Cv_pixel1->pb + Cv_pixel2->pb + Cv_pixel3->pb + Cv_pixel4->pb) / 4.0;
        averages.avg_pr = (Cv_pixel1->pr + Cv_pixel2->pr + Cv_pixel3->pr + Cv_pixel4->pr) / 4.0;
        averages.a = (Cv_pixel4->y + Cv_pixel3->y + Cv_pixel2->y + Cv_pixel1->y) / 4.0;
        averages.b = (Cv_pixel4->y + Cv_pixel3->y - Cv_pixel2->y - Cv_pixel1->y) / 4.0;
        averages.c = (Cv_pixel4->y - Cv_pixel3->y + Cv_pixel2->y - Cv_pixel1->y) / 4.0;
        averages.d = (Cv_pixel4->y - Cv_pixel3->y - Cv_pixel2->y + Cv_pixel1->y) / 4.0;
        return averages;
}


int64_t quantizeValues(float value, float range_limits)
{       
        if (fabsf(value) <= range_limits) {
                return (int64_t)round(value / range_limits * 15);
        } else {
                return value > 0 ? 15 : -15;
        }
}
