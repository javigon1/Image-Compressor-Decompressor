#include "rgb_cv.h"
#include "cv_dct.h"
#include "bitpack.h"
#include "calculations.h"
#include "image.h"
#include "uarray2.h"
#include "packUnpack.h"
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

        Pnm_ppm rgb_image = cv_to_rgb(back_to_cv, map, methods);

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
        // methods->free(&back_to_cv);
        // methods->free(&wordImage);
        // methods->free(&ba ck_to_rgb);
}
/* /comp/40/bin/images/flowers.ppm */

void packUnpack()
{
        FILE* fp = fopen("ppm.ppm", "rb");

        if (!fp) {
                fprintf(stderr, 
                        "One of the files provided couldn't be opened\n");
                exit(EXIT_FAILURE);
        }

        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        fprintf(stderr, "1\n");

        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        Pnm_ppm image = readImagePpm(fp, methods);
        fclose(fp);
        assert(image);

        A2Methods_UArray2 cv_image = rgb_to_cv(image, map, methods);
        
        fprintf(stderr, "2\n");

        A2Methods_UArray2 wordImage = cv_to_DCT(cv_image, map, methods);

         fprintf(stderr, "3\n");

        A2Methods_UArray2 packed = packWords(wordImage, map, methods);

         fprintf(stderr, "4\n");

        A2Methods_UArray2 unpacked = unpackWords(packed, map, methods);

         fprintf(stderr, "5\n");

        A2Methods_UArray2 back_to_cv = DCT_to_cv(unpacked, map, methods);
        
         fprintf(stderr, "6\n");

        Pnm_ppm rgb_image = cv_to_rgb(back_to_cv, map, methods);

         fprintf(stderr, "7\n");

        Pnm_ppmwrite(stdout, rgb_image);
        
        fprintf(stderr, "8\n");

        // Pnm_ppmfree(&image);  
        // methods->free(&back_to_cv);
        // methods->free(&wordImage);
        // methods->free(&packed);
        // methods->free(&unpacked);
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

void fitsu()
{
        printf("Bitpack_fitsu test (1): %d\n", Bitpack_fitsu(7, 3));
}

void fitss()
{
        printf("Bitpack_fitss test (1): %d\n", Bitpack_fitss(3, 3));
}

void fitsu2()
{
        printf("Bitpack_fitsu test2 (0): %d\n", Bitpack_fitsu(8, 3));
}

void fitss2()
{
        printf("Bitpack_fitss test2 (0): %d\n", Bitpack_fitss(4, 3));
}

void fitsu3()
{
        printf("Bitpack_fitsu test3 (1): %d\n", Bitpack_fitsu(0, 3));
}

void fitss3()
{
        printf("Bitpack_fitss test3 (1): %d\n", Bitpack_fitss(-4, 3));
}

void fitsu4()
{
        printf("Bitpack_fitsu test4 (1): %d\n", Bitpack_fitsu(-1, 3));
}

void fitss5()
{
        printf("Bitpack_fitss test5 (0): %d\n", Bitpack_fitss(-5, 3));
}

void fitss6()
{
        printf("Bitpack_fitss test6 (1): %d\n", Bitpack_fitss(-9, 5));
}

void fitss7()
{
        printf("Bitpack_fitss test7 (0): %d\n", Bitpack_fitss(-9, 4));
}

void fitss8()
{
        printf("Bitpack_fitss test8 (0): %d\n", Bitpack_fitss(-275, 5));
}

void test_getu()
{
        uint64_t word = 0x3f4; 
        unsigned width = 6;   
        unsigned lsb = 2;
        uint64_t field = Bitpack_getu(word, width, lsb);
        printf("FIELD UNSIGNED: %" PRIu64"\n", field);
}

void test_getu2()
{
        uint64_t word = 14585;
        unsigned width = 7;   
        unsigned lsb = 0;
        uint64_t field = Bitpack_getu(word, width, lsb);
        printf("FIELD UNSIGNED: (121)%" PRIu64"\n", field);
}

void test_gets()
{
        int64_t word = 0x3f4;
        unsigned width = 6; 
        unsigned lsb = 2;
        int64_t field = Bitpack_gets(word, width, lsb);
        printf("FIELD SIGNED: %" PRId64"\n", field);
}

void test_gets2()
{
        uint64_t word = -20;
        unsigned width = 4;   
        unsigned lsb = 0;
        uint64_t field = Bitpack_gets(word, width, lsb);
        printf("FIELD UNSIGNED: (12)%" PRId64"\n", field);
}

void test_gets3()
{
        uint64_t word = -124;
        unsigned width = 9;   
        unsigned lsb = 5;
        uint64_t field = Bitpack_gets(word, width, lsb);
        printf("FIELD UNSIGNED: (4!!!!)%" PRId64"\n", field);
}

void test_gets4()
{
        uint64_t word = 357;
        unsigned width  = 4;  
        unsigned lsb = 0;
        uint64_t field = Bitpack_gets(word, width, lsb);
        printf("FIELD SIGNED: %" PRId64 "\n", field);
}

/* Works correctly */
void RaiseException()
{
        int64_t word = 14585;
        unsigned width = 3;
        unsigned lsb = 0;
        uint64_t value = 67;
        uint64_t field = Bitpack_newu(word, width, lsb, value);
        printf("new uint64_t (14531): %lu\n", field);
}

/* Works correctly */
void RaiseException2()
{
        int64_t word = 14585;
        unsigned width = 3;
        unsigned lsb = 0;
        uint64_t value = 67;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t: %lu\n", field);
}

/* Works correctly */
void RaiseException3()
{
        int64_t word = 14585;
        unsigned width = 3;
        unsigned lsb = 0;
        uint64_t value = 7;
        uint64_t field = Bitpack_newu(word, width, lsb, value);
        printf("new uint64_t (14531): %lu\n", field);
        assert(field == 14531);
}


void test_newu()
{
        int64_t word = 14585;
        unsigned width = 7;
        unsigned lsb = 0;
        uint64_t value = 67;
        uint64_t field = Bitpack_newu(word, width, lsb, value);
        printf("new uint64_t (14531): %lu\n", field);
}


void test_newu2()
{
        int64_t word = 0x7FFFFFFFFFFFFFFF;
        unsigned width = 64;
        unsigned lsb = 0;
        uint64_t value = 0;
        uint64_t field = Bitpack_newu(word, width, lsb, value);
        printf("new uint64_t (0): %lu\n", field);

}

void test_newu3()
{
        int64_t word = 53675;
        unsigned width = 3;
        unsigned lsb = 3;
        uint64_t value = 6;
        uint64_t field = Bitpack_newu(word, width, lsb, value);
        printf("new uint64_t (53683): %lu\n", field);
}

void test_news()
{
        uint64_t word = 17;
        unsigned width = 3;
        unsigned lsb = 2;
        int64_t value = 3;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t (13): %ld\n", field);
}

void test_news2()
{
        uint64_t word = -20;
        unsigned width = 5;
        unsigned lsb = 5;
        int64_t value = 10;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t2 (-692): %ld\n", field);
}

void test_pack()
{
        uint64_t x = 4278255360;
        uint64_t field = Bitpack_getu(x, 8, 0);
        uint64_t field2 = Bitpack_getu(x, 8, 8);
        uint64_t field3 = Bitpack_getu(x, 8, 16);
        uint64_t field4 = Bitpack_getu(x, 8, 24);

        printf("Value1: %lu\n", field);
        printf("Value2: %lu\n", field2);
        printf("Value3: %lu\n", field3);
        printf("Value4: %lu\n", field4);

        print_uint64_binary(field);
        print_uint64_binary(field2);
        print_uint64_binary(field3);
        print_uint64_binary(field4);

}

void test_news3()
{
        uint64_t word = -9;
        unsigned width = 3;
        unsigned lsb = 0;
        int64_t value = -2;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t3 (-10): %ld\n", field);
}

void test_news4()
{
        uint64_t word = -17;
        unsigned width = 4;
        unsigned lsb = 0; 
        int64_t value = 7;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t4 (-25): %ld\n", field);
}

void test_news5()
{
        uint64_t word = -123456;
        unsigned width = 6;
        unsigned lsb = 5;
        int64_t value = -20;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t5 (-123520): %ld\n", field);
}

void test_news6()
{
        uint64_t word = -500;
        unsigned width = 7;
        unsigned lsb = 1;
        int64_t value = -20;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t6 (-112): %ld\n", field);
}

void test_news7()
{
        uint64_t word = -17;
        unsigned width = 4;
        unsigned lsb = 0;
        int64_t value = -7;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t4 (-23): %ld\n", field);
}

void test_news8()
{
        uint64_t word = -17;
        unsigned width = 4;
        unsigned lsb = 0;
        int64_t value = 7;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t4 (-25): %ld\n", field);
}

/* SHOULD RESULT IN FAILED ASSERTION */
void test_news9()
{
        uint64_t word = -123456;
        unsigned width = 5;
        unsigned lsb = 5;
        int64_t value = -275;
        uint64_t field = Bitpack_news(word, width, lsb, value);
        printf("new int64_t5 (-15136): %ld\n", field);
}

void test_law1()
{
        uint64_t word = 500;
        unsigned width = 5;
        unsigned lsb = 1;
        uint64_t value = 20;
        uint64_t left = Bitpack_getu(Bitpack_newu(word, width, lsb, value), width, lsb);
        uint64_t right = value;
        printf("right: %ld, left: %ld\n", right, left);
}

void test_law2()
{
        uint64_t word = -500;
        unsigned width = 5;
        unsigned lsb = 1;
        uint64_t value = -20;
        uint64_t left = Bitpack_gets(Bitpack_news(word, width, lsb, value), width, lsb);
        uint64_t right = value;
        printf("right: %ld, left: %ld\n", right, left);
}

// void test_law2()
// {
//         //lsb2 >= w + lsb,
//         uint64_t word = -500;
//         unsigned width = 7;
//         unsigned lsb = 1;
//         int64_t value = -20;
//         uint64_t left = getu(newu(word, width, lsb, value), width2, width + lsb); 
//         uint64_t right = getu(word, width2, width + lsb);

//         printf("right: %ld, left: %ld\n\n", right, left);
// }

void testPack()
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

        A2Methods_UArray2 wordImage = cv_to_DCT(cv_image, map, methods);

        for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                        code_word word = (code_word)methods->at(wordImage, j, i);
                        unsigned a = word->a;
                        signed b = word->b;
                        signed c = word->c;
                        signed d = word->d;
                        unsigned pb = word->pb;
                        unsigned pr = word->pr;
                        printf("[%d][%d]\n", j, i);
                        fprintf(stderr, "a: %u, b: %d, c: %d, d: %d, pb: %u, pr: %u\n",
                                a, b, c, d, pb, pr);
                }
        }

        A2Methods_UArray2 wordSequence = packWords(wordImage, map, methods);

        for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                        uint64_t *packed_word = methods->at(wordSequence, i, j);
                        printf("Packed word at (%d, %d): %lu\n", i, j, *packed_word);
                }
        }

        Pnm_ppmfree(&image);  
        methods->free(&wordSequence);
        methods->free(&wordImage);
}