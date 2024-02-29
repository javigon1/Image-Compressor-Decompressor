#include "image.h"


#define A2M A2Methods_UArray2


Pnm_ppm readImagePpm(FILE *fp, A2Methods_T methods) 
{
        Pnm_ppm image = Pnm_ppmread(fp, methods);

        bool oddDimensions = false;
        A2M new_image;

        if (image->width % 2 != 0) {
                (image->width)--;
                oddDimensions = true;
        }

        if (image->height % 2 != 0) {
                (image->height)--;
                oddDimensions = true;
        }

        if (oddDimensions == false) {
                return image;
        } else {
                new_image = methods->new(methods->width(image), 
                                              methods->height(image),
                                              sizeof(struct Pnm_rgb));
                for (int i = 0; i < methods->width(new_image); i++) {
                        for (int j = 0; j < methods->height(new_image); j++) {
                                Pnm_rgb pixel = methods->at(image->pixels, i, j);
                                Pnm_rgb new_pixel = methods->at(new_image, i, j);
                                new_pixel->red = pixel->red;
                                new_pixel->blue = pixel->blue;
                                new_pixel->green = pixel->green;
                        }
                }
        }

        image->pixels = new_image;
        methods->free(&(image->pixels));
        
        
        return new_image;
}       


void printImagePpm(A2Methods_UArray2 array, A2Methods_T methods)
{
        (void)array;
        (void)methods;

        // /* REMEBER TO MULTIPLE LATER - THIS IS JUST FOR CHECKING */
        // int width = methods->width(array);
        // int height = methods->height(array);

        // printf("COMP40 Compressed image format 2\n%u %u\n", width, height);

        // for (int i = 0; i < height; i++) {
        //         for (int j = 0; j < width; j++) {
        //                 printf("") methods->at(array, j, i);
        //         }
        // }


}