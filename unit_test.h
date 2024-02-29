#include "RGB_component.h"
#include "image.h"


// typedef struct Pnm_rgb {
//         unsigned red, green, blue;
// } *Pnm_rgb;

/*PRINT FUNCTIONS FOR TESTING*/
/*Print all cells of plain uarray2 with mapping*/
void a2_print(int i, int j, A2Methods_UArray2 a, A2Methods_Object *p1, void *p2)
{
        extern A2Methods_T uarray2_methods_plain;
        (void)p1;
        (void)p2;
        int num = *((int *)uarray2_methods_plain->at(a, i, j));
        printf("arr[%d,%d]: %d\n", i, j, num);
}

/*Print all cells of blocked uarray2b mapping*/
void a2b_print(int i, int j, UArray2b_T a, void *p1, void *p2)
{
        (void)p1;
        (void)p2;
        int num = *((int *)UArray2b_at(a, i, j));
        printf("arr[%d,%d]: %d\n", i, j, num);
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

// void isImage(){
//         FILE *fp;
//         fp = fopen("", "rb")
// }

void RGB1()
{
        Pnm_rgb pixel = malloc(sizeof(struct Pnm_rgb));
        assert(pixel);
        pixel->red = 27;
        pixel->blue = 288;
        pixel->green = 249;
        Pnm_ypbpr new_pixel;
        //= malloc(sizeof(*new_pixel));
        new_pixel = rgb_to_cv(pixel);
        printf("Y: %0.f\nPb: %0.f\nPr: %0.f\n", new_pixel->y, new_pixel->pb,
                                                new_pixel->pr);
}