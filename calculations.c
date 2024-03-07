#include "calculations.h"


float roundValues(float value, float lower_range, float higher_range)
{
        if (value < lower_range) return lower_range;
        if (value > higher_range) return higher_range;
        return value;
}


ChromaAverages computeAverageChromas(Pnm_ypbpr Cv1, Pnm_ypbpr Cv2, 
                                     Pnm_ypbpr Cv3, Pnm_ypbpr Cv4)
{
        ChromaAverages averages;
        averages.avg_pb = (Cv1->pb + Cv2->pb + Cv3->pb + Cv4->pb) / 4.0;
        averages.avg_pr = (Cv1->pr + Cv2->pr + Cv3->pr + Cv4->pr) / 4.0;
        averages.a = (Cv4->y + Cv3->y + Cv2->y + Cv1->y) / 4.0;
        averages.b = (Cv4->y + Cv3->y - Cv2->y - Cv1->y) / 4.0;
        averages.c = (Cv4->y - Cv3->y + Cv2->y - Cv1->y) / 4.0;
        averages.d = (Cv4->y - Cv3->y - Cv2->y + Cv1->y) / 4.0;
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
