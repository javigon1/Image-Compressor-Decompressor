#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "pnm.h"
#include "a2plain.h"
#include "uarray2.h"
#include "a2methods.h"
#include "assert.h"


#define A2U A2Methods_UArray2
#define A2M A2Methods_T 


double checkDiff(Pnm_ppm file1, Pnm_ppm file2, A2M methods);
bool checkDim(A2U image1, A2U image2, A2M methods);


int main(int argc, char *argv[])
{
        if (argc > 3) {
                fprintf(stderr, "Too many arguments provided");
                exit(1);
        }
        
        FILE* file1;
        FILE* file2;

        if (strcmp(argv[1], "-") == 0) {
                file1 = stdin;
        } else if (strcmp(argv[2], "-") == 0) {
                file2 = stdin;
        }

        file1 = fopen(argv[1], "rb");
        file2 = fopen(argv[2], "rb");

        if (!file1 || !file2) {
                fprintf(stderr, 
                        "One of the files provided couldn't be opened\n");
                exit(EXIT_FAILURE);
        }

        A2M methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm ppm1 = Pnm_ppmread(file1, methods);
        fclose(file1);
        assert(ppm1);
        Pnm_ppm ppm2 = Pnm_ppmread(file2, methods);
        fclose(file2);
        assert(ppm2);

        double diff = checkDiff(ppm1, ppm2, methods);

        printf("%f\n", diff);

        return EXIT_SUCCESS;
}


double checkDiff(Pnm_ppm file1, Pnm_ppm file2, A2M methods)
{
        A2U image1;
        A2U image2; 
        
        image1 = methods->new(methods->width(file1), 
                                         methods->height(file1),
                                         sizeof(struct Pnm_rgb));
        image2 = methods->new(methods->width(file2), 
                                         methods->height(file2),
                                         sizeof(struct Pnm_rgb));
        if (!checkDim(image1, image2, methods)) {
                fprintf(stderr, "Dimensions differ by more than 1\n");
                return EXIT_FAILURE;
        } 

        image1 = file1->pixels;
        image2 = file2->pixels;
        double maxval1 = file1->denominator;
        double maxval2 = file2->denominator;

        double sum = 0;

        double small_width = methods->width(image2);
        double small_height = methods->height(image2);
        if (methods->width(image1) <= methods->width(image2)){
                small_width = methods->width(image1);
        }
        if (methods->height(image1) <= methods->height(image2))
        {
                small_height = methods->height(image1);
        }

        for (int i = 0; i < small_width; i++) {
                for (int j = 0; j < small_height; j++) {
                        Pnm_rgb pixel1 = methods->at(image1, i, j);
                        Pnm_rgb pixel2 = methods->at(image2, i, j);
                        
                        double green1 = (double)pixel1->green;
                        double green2 = (double)pixel2->green;
                        double blue1 = (double)pixel1->blue;
                        double blue2 = (double)pixel2->blue;
                        double red1 = (double)pixel1->red;
                        double red2 = (double)pixel2->red;
                        sum += ((green1 / maxval1) - (green2 / maxval2)) * ((green1 / maxval1) - (green2 / maxval2)) +
                        ((red1 / maxval1) - (red2 / maxval2)) * ((red1 / maxval1) - (red2 / maxval2)) +
                        ((blue1 / maxval1) - (blue2 / maxval2)) * ((blue1 / maxval1) - (blue2 / maxval2));
                }
        }

        return sqrt(sum / (3 * small_height * small_width)); 
}

bool checkDim(A2U image1, A2U image2, A2M methods)
{
        if (abs(methods->height(image1) - methods->height(image2)) > 1 ) {
                fprintf(stderr, "%0.f", 1.0);
                return false;
        }
        
        if (abs(methods->width(image1) - methods->width(image2)) > 1 ) {
                fprintf(stderr, "%0.f", 1.0);
                return false;
        }

        return true;
}


