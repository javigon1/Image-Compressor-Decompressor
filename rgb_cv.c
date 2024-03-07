#include "rgb_cv.h"
#include "calculations.h"


#define MAXVAL 255


A2Methods_UArray2 rgb_to_cv(Pnm_ppm image, A2Methods_mapfun *map,
                            A2Methods_T methods)
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

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return cv_image;
}

void applyRGB(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;

        struct closure *closure = (struct closure *)cl;
        unsigned denom = closure->denominator;

        Pnm_rgb rgb_pix = elem;
        float red = (float)rgb_pix->red / denom;
        float blue = (float)rgb_pix->blue / denom;
        float green = (float)rgb_pix->green / denom;

        float y = 0.299 * red + 0.587 * green + 0.114 * blue;
        float pb = -0.168736 * red - 0.331264 * green + 0.5 * blue;
        float pr = 0.5 * red - 0.418688 * green - 0.081312 * blue;

        Pnm_ypbpr ypbpr_pix = (Pnm_ypbpr)closure->methods->at(closure->array, col, row);
        ypbpr_pix->y = roundValues(y, 0, 1);
        ypbpr_pix->pb = roundValues(pb, -0.5, 0.5);
        ypbpr_pix->pr = roundValues(pr, -0.5, 0.5);
}


Pnm_ppm cv_to_rgb(A2Methods_UArray2 image, A2Methods_mapfun *map, A2Methods_T methods)
{
        int width = methods->width(image);
        int height = methods->height(image);
        
        Pnm_ppm rgb_image;
        NEW(rgb_image);

        rgb_image->width = width;
        rgb_image->height = height;
        rgb_image->denominator = MAXVAL;
        rgb_image->methods = methods;
        rgb_image->pixels = methods->new(width, height, 
                                         sizeof(struct Pnm_ypbpr));

        struct closure *cl = malloc(sizeof(*cl));

        cl->methods = methods;
        cl->array = rgb_image->pixels;
        /* check for this being the limit */
        cl->denominator = MAXVAL;

        map(image, applyCV, cl);

        if(cl) {
                free(cl);
                cl = NULL;
        }

        return rgb_image;
}


void applyCV(int col, int row, A2Methods_UArray2 UArray2, void *elem, void *cl)
{
        (void)UArray2;

        Pnm_ypbpr ypbpr = elem;

        struct closure *closure = (struct closure *)cl;

        float y = ypbpr->y;
        float pb = ypbpr->pb;
        float pr = ypbpr->pr;

        float r = roundValues(1.0 * y + 0.0 * pb + 1.402 * pr, 0, 1);
        float g = roundValues(1.0 * y - 0.344136 * pb - 0.714136 * pr, 0 ,1);
        float b = roundValues(1.0 * y + 1.772 * pb + 0.0 * pr, 0, 1);

        Pnm_rgb rgb_pix = (Pnm_rgb)closure->methods->at(closure->array, col, row);
        rgb_pix->red = (unsigned)(r * MAXVAL);
        rgb_pix->green = (unsigned)(g * MAXVAL);
        rgb_pix->blue = (unsigned)(b * MAXVAL);
}
        