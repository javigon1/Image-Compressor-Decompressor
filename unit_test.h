#include "RGB_component.h"
#include "CV_to_DCT.h"
#include "bitpack.h"
#include "calculations.h"
#include "image.h"
#include "uarray2.h"
#include "uarray.h"


void testRGBtoCV()
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

        printf("Width: %d\n", image->width);
        printf("Height: %d\n", image->height);
        printf("Denom: %d\n", image->denominator);

        A2Methods_UArray2 cv_image = rgb_to_cv(image, map, methods);

        for (int i = 0; i < 10; i++) {
                Pnm_rgb first_pixel = (Pnm_rgb)methods->at(image->pixels, i, 0);
                unsigned red = first_pixel->red;
                unsigned green = first_pixel->green;
                unsigned blue = first_pixel->blue;
                printf("Red: %u, Green: %u, Blue: %u\n", red, green, blue);
        }

        for (int i = 0; i < 10; i++){
                Pnm_ypbpr first_cv = (Pnm_ypbpr)methods->at(cv_image, i, 0);
                float y = first_cv->y;
                float pb = first_cv->pb;
                float pr = first_cv->pr;
                printf("y: %f, pb: %f, pr: %f\n", y, pb, pr);
        }  

        Pnm_ppmfree(&image);  
        methods->free(&cv_image);
}

void testCVtoRGB() 
{
        FILE* fp = fopen("ppm.ppm", "rb");

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

        A2Methods_UArray2 cv_image = rgb_to_cv(image, map, methods);

        Pnm_ppm rgb_image = cv_to_rgb(cv_image, map, methods);

        Pnm_ppmwrite(stdout, rgb_image); 
}


void DCT()
{
        FILE* fp = fopen("ppm.ppm", "rb");

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

        A2Methods_UArray2 cv_image = rgb_to_cv(image, map, methods);

        for (int i = 0; i < 10; i+=2) {
                Pnm_ypbpr ypbpr_pixel = (Pnm_ypbpr)methods->at(cv_image, i, 0);
                float y = ypbpr_pixel->y;
                float pb = ypbpr_pixel->pb;
                float pr = ypbpr_pixel->pr;
                fprintf(stderr, "y: %f, pb: %f, pr: %f\n", y, pb, pr);
        }

        A2Methods_UArray2 wordImage = cv_to_DCT(cv_image, map, methods);

        for (int i = 0; i < 5; i++) {
                code_word word = (code_word)methods->at(wordImage, i, 0);
                unsigned a = word->a;
                signed b = word->b;
                signed c = word->c;
                signed d = word->d;
                unsigned pb = word->pb;
                unsigned pr = word->pr;
                fprintf(stderr, "a: %u, b: %d, c: %d, d: %d, pb: %u, pr: %u\n",
                                 a, b, c, d, pb, pr);
        }

        Pnm_ppmfree(&image);  
        methods->free(&cv_image);
        methods->free(&wordImage);
}


void testAvgLuminance()
{
        Pnm_ypbpr pixel1 = malloc(sizeof(*pixel1));
        if (!pixel1) {
                fprintf(stderr, "Error: Unable to allocate memory.\n");
                exit(EXIT_FAILURE);
        }
        pixel1->y = 0.30;
        pixel1->pb = 0.2;
        pixel1->pr = -0.13;

        Pnm_ypbpr pixel2 = malloc(sizeof(*pixel2));
        if (!pixel2) {
                fprintf(stderr, "Error: Unable to allocate memory.\n");
                exit(EXIT_FAILURE);
        }
        pixel2->y = 0.17;
        pixel2->pb = 0.12;
        pixel2->pr = 0.24;

        Pnm_ypbpr pixel3 = malloc(sizeof(*pixel3));
        if (!pixel3) {
                fprintf(stderr, "Error: Unable to allocate memory.\n");
                exit(EXIT_FAILURE);
        }
        pixel3->y = 0.78;
        pixel3->pb = 0.23;
        pixel3->pr = -0.09;

        Pnm_ypbpr pixel4 = malloc(sizeof(*pixel4));
        if (!pixel4) {
                fprintf(stderr, "Error: Unable to allocate memory.\n");
                exit(EXIT_FAILURE);
        }
        pixel4->y = 0.90;
        pixel4->pb = 0.1;
        pixel4->pr = -0.1;

        ChromaAverages averages = computeAverageChromas(pixel1, pixel2, pixel3, pixel4);
        float avg_pb = averages.avg_pb;
        float avg_pr = averages.avg_pr;
        float a = averages.a;
        float b = averages.b;
        float c = averages.c;
        float d = averages.d;

        printf("Average Pb: %0.2f\nAverage Pr: %0.2f\n", avg_pb, avg_pr);
        printf("a: %0.2f\nb: %0.2f\nc: %0.2f\nd: %0.2f\n", a, b, c, d);

        free(pixel1);
        free(pixel2);
        free(pixel3);
        free(pixel4);
}


void DCTtoCv()
{
        FILE* fp = fopen("ppm.ppm", "rb");

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

        // printf("IN RGB\n");

        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 4; j++) {
        //                 Pnm_rgb first_pixel = (Pnm_rgb)methods->at(image->pixels, j, i);
        //                 unsigned red = first_pixel->red;
        //                 unsigned green = first_pixel->green;
        //                 unsigned blue = first_pixel->blue;
        //                 printf("[%d][%d]\n",j, i);
        //                 printf("Red: %u, Green: %u, Blue: %u\n", red, green, blue);
        //         }
        // }

        A2Methods_UArray2 cv_image = rgb_to_cv(image, map, methods);

        // printf("RGB TO CV\n");

        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 4; j++) {
        //                 Pnm_ypbpr ypbpr_pixel = (Pnm_ypbpr)methods->at(cv_image, j, i);
        //                 float y = ypbpr_pixel->y;
        //                 float pb = ypbpr_pixel->pb;
        //                 float pr = ypbpr_pixel->pr;
        //                 printf("[%d][%d]\n",j, i);
        //                 fprintf(stderr, "y: %f, pb: %f, pr: %f\n", y, pb, pr);
        //         }
        // }

        A2Methods_UArray2 wordImage = cv_to_DCT(cv_image, map, methods);

        // printf("CV TO DCT\n");

        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 2; j++) {
        //                 code_word word = (code_word)methods->at(wordImage, j, i);
        //                 unsigned a = word->a;
        //                 signed b = word->b;
        //                 signed c = word->c;
        //                 signed d = word->d;
        //                 unsigned pb = word->pb;
        //                 unsigned pr = word->pr;
        //                 printf("[%d][%d]\n", j, i);
        //                 fprintf(stderr, "a: %u, b: %d, c: %d, d: %d, pb: %u, pr: %u\n",
        //                         a, b, c, d, pb, pr);
        //         }
        // }


        A2Methods_UArray2 back_to_cv = DCT_to_cv(wordImage, map, methods);

        // printf("DCT TO CV\n");

        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 4; j++) {
        //                 Pnm_ypbpr ypbpr_pixel = (Pnm_ypbpr)methods->at(back_to_cv, j, i);
        //                 float y = ypbpr_pixel->y;
        //                 float pb = ypbpr_pixel->pb;
        //                 float pr = ypbpr_pixel->pr;
        //                 printf("[%d][%d]\n",j, i);
        //                 fprintf(stderr, "y: %f, pb: %f, pr: %f\n", y, pb, pr);
        //         }
        // }

        Pnm_ppm rgb_image = cv_to_rgb(cv_image, map, methods);

        Pnm_ppmwrite(stdout, rgb_image); 

        // printf("BACK TO RGB\n");


        // for (int i = 0; i < 2; i++) {
        //         for (int j = 0; j < 4; j++) {
        //                 Pnm_rgb first_pixel = (Pnm_rgb)methods->at(image->pixels, j, i);
        //                 unsigned red = first_pixel->red;
        //                 unsigned green = first_pixel->green;
        //                 unsigned blue = first_pixel->blue;
        //                 printf("[%d][%d]\n",j, i);
        //                 printf("Red: %u, Green: %u, Blue: %u\n", red, green, blue);
        //         }
        // }

        // (void)back_to_rgb;

        Pnm_ppmfree(&image);  
        methods->free(&back_to_cv);
        methods->free(&wordImage);
        // methods->free(&ba ck_to_rgb);
}


void testShifts()
{
        uint64_t test_u = 0x3f4;
        int64_t test_s = 0x3f4;
        unsigned width = 11;
        // unsigned lsb = 2;

        printf("Value unsigned: %lu\n", test_u);
        printf("Value unsigned: %ld\n", test_s);

        printf("Bitpack_fitsu test: %d\n", Bitpack_fitsu(test_u, width));
        printf("Bitpack_fitss test: %d\n", Bitpack_fitss(test_s, width));
}

void testShifts2()
{       
        printf("Bitpack_fitsu test (TRUE): %d\n", Bitpack_fitsu(5, 3));
        printf("Bitpack_fitss test (FALSE): %d\n", Bitpack_fitss(5, 3));
}

void testExtract()
{
        uint64_t result = Bitpack_getu(0x3f4, 6, 2);
        printf("Bitpack_getu test (61): %lu\n", result);
        int64_t result2 = Bitpack_gets(0x3f4, 6, 2);
        printf("Bitpack_gets test (-3): %ld\n", result2);
}

void test_getu()
{
        uint64_t word = 0x3f4; // 001111110100
        unsigned width = 6;    // ----^^^^^^--
        unsigned lsb = 2;
        uint64_t field = Bitpack_getu(word, width, lsb);
        printf("FIELD UNSIGNED: %" PRIu64"\n", field);
        assert(field == 61);
}

void test_gets()
{
        int64_t word = 0x3f4; // 001111110100
        unsigned width = 6;   // ----^^^^^^--
        unsigned lsb = 2;
        int64_t field = Bitpack_gets(word, width, lsb);
        printf("FIELD SIGNED: %" PRId64"\n", field);
        assert(field == -3);

        word = 357; // 101100101
        width = 4;  // -----^^^^
        lsb = 0;
        field = Bitpack_gets(word, width, lsb);
        printf("FIELD SIGNED: %" PRId64 "\n", field);
        assert(field == 5);
}

void test_newu()
{
        int64_t word = 14585;
        unsigned width = 7;
        unsigned lsb = 0;
        uint64_t value = 67;
        uint64_t field = Bitpack_newu(word, width, lsb, value);
        printf("new uint64_t: %lu\n", field);
        assert(field == 14531);
}

void test_news()
{
        int64_t word = -17;
        unsigned width = 2;
        unsigned lsb = 2;
        uint64_t value = 3;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t: %lu\n", field);
        //assert(field == );
}

// void testPower()
// {
//         printf("Value: %u\n", power(2, 0));
// }