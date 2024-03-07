#include "calculations.h"

/*
 * roundValues
 * Description: Takes a float and quantizes it by setting it equal to the
 *              upper or lower limit of a range if it exceeds said range
 * Parameters:  float value - the value to quantize
 *              float lower_range - the lower limit
 *              float higher_range - the upper limit
 * Return:      value if it falls in the range, higher_range if greater than
 *              higher_range, lower_range if less than lower_range
 * Expects:     
 * Notes:
 *
 */
float roundValues(float value, float lower_range, float higher_range)
{
        /* checks if the value is out of ranges and assigns the closest 
        range */
        if (value < lower_range) return lower_range;
        if (value > higher_range) return higher_range;
        return value;
}

/*
 * computeAverageChromas
 * Description: Takes four instances of component value structs and returns a
 *              struct containing the averages of the respective values of
 *              each component value struct member
 * Parameters:  Pnm_ypbpr Cv_pixel1, 2, 3, 4 - the four component value pixels
 * Return:      ChromaAverages struct instance containing the averages of the
 *              members of Cv_pixel1...4
 * Expects: Cv_pixel1...4 to not be NULL
 * Notes:
 *
 */
ChromaAverages computeAverageChromas(Pnm_ypbpr Cv1, Pnm_ypbpr Cv2, 
                                     Pnm_ypbpr Cv3, Pnm_ypbpr Cv4)
{
        /* store everything in a struct and return it */
        ChromaAverages averages;
        averages.avg_pb = (Cv1->pb + Cv2->pb + Cv3->pb + Cv4->pb) / 4.0;
        averages.avg_pr = (Cv1->pr + Cv2->pr + Cv3->pr + Cv4->pr) / 4.0;
        /* perform the arithmetic given */
        averages.a = (Cv4->y + Cv3->y + Cv2->y + Cv1->y) / 4.0;
        averages.b = (Cv4->y + Cv3->y - Cv2->y - Cv1->y) / 4.0;
        averages.c = (Cv4->y - Cv3->y + Cv2->y - Cv1->y) / 4.0;
        averages.d = (Cv4->y - Cv3->y - Cv2->y + Cv1->y) / 4.0;
        return averages;
}

/*
 * quantizeValues
 * Description: Takes a float and scales it by dividing it by the range limit
 *              and then quantizes it by rounding to the nearest integer
 * Parameters:  float value - the value to quantize
 *              float range_limits - the outermost values of the range
 * Return:      the nearest integer to value after scaling, or 15 if greater
 *              than the ceiling of 15, -15 if less than the floor of -15
 * Expects:
 * Notes:
 *
 */
int64_t quantizeValues(float value, float range_limits)
{       
        /* same logic as roundValues but returns and int64_t */
        if (fabsf(value) <= range_limits) {
                return (int64_t)round(value / range_limits * 15);
        } else {
                return value > 0 ? 15 : -15;
        }
}
