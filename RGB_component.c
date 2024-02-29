#include "RGB_component.h"

float round_values(float value, float lower_range, float higher_range)
{
        if (value < lower_range) return lower_range;
        if (value > higher_range) return higher_range;
        return value;
}


A2Methods_UArray2 rgb_to_cv(Pnm_ppm image, 
                            A2Methods_mapfun *map, A2Methods_T methods)
{
        A2Methods_UArray2 cv_image = methods->new(methods->width(image), 
                                                   methods->height(image),
                                                   sizeof(struct Pnm_ypbpr));

        struct closure *cl = malloc(sizeof(*cl));
        cl->methods = methods;
        cl->array = cv_image;
        /* check for this being the limit */
        cl->denominator = image->denominator;

        map(image->pixels, applyRGB, cl);

        if(!cl) {
                free(cl);
                cl = NULL;
        }

        return cv_image;
}

void applyRGB(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;

        struct closure *closure = (struct closure *)cl;
        unsigned int denom = closure->denominator;

        Pnm_rgb rgb_pix = elem;
        float red = (float)rgb_pix->red / denom;
        float blue = (float)rgb_pix->blue / denom;
        float green = (float)rgb_pix->green / denom;

        float y = 0.299 * red + 0.587 * green + 0.114 * blue;
        float pb = -0.168736 * red - 0.331264 * green + 0.5 * blue;
        float pr = 0.5 * red - 0.418688 * green - 0.081312 * blue;

        Pnm_ypbpr ypbpr_pix = (Pnm_ypbpr)closure->methods->at(closure->array, col, row);
        ypbpr_pix->y = round_values(y, 0, 1);
        ypbpr_pix->pb = round_values(pb, -0.5, 0.5);
        ypbpr_pix->pr = round_values(pr, -0.5, 0.5);
}


    
        