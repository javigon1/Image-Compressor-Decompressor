#include "RGB_component.h"
#include "image.h"
#include "uarray2.h"
#include "uarray.h"


// typedef struct Pnm_rgb {
//         unsigned red, green, blue;
// } *Pnm_rgb;

/*PRINT FUNCTIONS FOR TESTING*/
/*Print all cells of plain uarray2 with mapping*/
// void a2_print(int i, int j, A2Methods_UArray2 a, A2Methods_Object *p1, void *p2)
// {
//         extern A2Methods_T uarray2_methods_plain;
//         (void)p1;
//         (void)p2;
//         int num = *((int *)uarray2_methods_plain->at(a, i, j));
//         // printf("arr[%d,%d]: %d\n", i, j, num);
// }

/*Print all cells of blocked uarray2b mapping*/
// void a2b_print(int i, int j, UArray2_T a, void *p1, void *p2)
// {
//         (void)p1;
//         (void)p2;
//         int num = *((int *)UArray2_at(a, i, j));
//         // printf("arr[%d,%d]: %d\n", i, j, num);
// }
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

// void isImage(char *img)
// {
//         A2Methods_T methods = uarray2_methods_plain;
//         FILE *fp;
//         fp = fopen(img, "rb"); // change parameter for image
//         Pnm_ppm image = readImagePpm(fp, methods);
//         printf("Width: %d", image->width);  // change parameter according to image
//         assert("Height: %d", image->height); // change parameter according to image
//         fclose(fp);
// }

void testYPBPR()
{
        FILE* fp = fopen("test.ppm", "rb");

        if (!fp) {
                fprintf(stderr, 
                        "One of the files provided couldn't be opened\n");
                exit(EXIT_FAILURE);
        }

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm image = readImagePpm(fp, methods);
        fclose(fp);
        assert(image);

        // printf("Width: %d\n", image->width);
        // printf("Height: %d\n", image->height);
        // printf("Denom: %d\n", image->denominator);

        A2Methods_UArray2 cv_image = rgb_to_cv(image, map, methods);


        // for (int i = 0; i < 10; i++) {
        //         Pnm_rgb first_pixel = (Pnm_rgb)methods->at(image->pixels, i, 0);
        //         unsigned red = first_pixel->red;
        //         unsigned green = first_pixel->green;
        //         unsigned blue = first_pixel->blue;
        //         printf("Red: %u, Green: %u, Blue: %u\n", red, green, blue);
        // }

        // for (int i = 0; i < 10; i++){
        //         Pnm_ypbpr first_cv = (Pnm_ypbpr)methods->at(cv_image, i, 0);
        //         float y = first_cv->y;
        //         float pb = first_cv->pb;
        //         float pr = first_cv->pr;
        //         printf("y: %f, pb: %f, pr: %f\n", y, pb, pr);
        // }

        Pnm_ppm rgb_image = cv_to_rgb(cv_image, map, methods);

        Pnm_ppmwrite(stdout, rgb_image);      
}

    // void RGB1()
    // {
    //     Pnm_ppm image = malloc(sizeof(*image));
    //     assert(image != NULL);
    //     image->denominator = 255;

    //     Pnm_rgb pixel = malloc(sizeof(*pixel));
    //     assert(pixel != NULL);
    //     pixel->red = 27;
    //     pixel->blue = 288;
    //     pixel->green = 249;

    //     A2Methods_mapfun *map = NULL;
    //     A2Methods_T methods = NULL;

    //     A2Methods_UArray2 cv_image = rgb_to_cv(image, map, methods);

    //     free(pixel);
    //     free(image);
    // }