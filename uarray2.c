

#include "uarray2.h"
#include "uarray.h"
#include "assert.h"
#include "mem.h"
#include "stdlib.h"

struct UArray2_T {
        int width; /*number of columns*/
        int height; /*number of rows*/
        int size; /*number of bytes*/
        UArray_T UArr;
};


/*
 * UArray2_new
 * Description: Builds a 2-dimensional UArray of the given dimensions,  
 *              reserving the indicated number of bytes for each element.
 * Parameters:  int col -- number of columns in the array
 *              int row -- number of rows in the array
 *              int bytes -- number of bytes allocated in memory for each
 *                           element
 * Return:      An empty "col" x "row" UArray_2 able to store elements of size
 *              "bytes"
 * Expects:     - col and row to be non-negative
 *              - bytes to be positive
 * Notes:
 *      - Will CRE if col or row is negative or if bytes is non-positive
 *      - Will also CRE if memory-related errors occur in allocating the array
 *      - Resulting UArray2 must be freed later using the Bit2_free function
 */
UArray2_T UArray2_new(int col, int row, int bytes)
{
        /*screening against invalid arguments*/
        assert(col >= 0);
        assert(row >= 0);
        assert(bytes > 0);

        /*initializing struct*/
        UArray2_T new_UA2 = ALLOC(sizeof(*new_UA2));

        new_UA2->width = col;
        new_UA2->height = row;
        new_UA2->size = bytes;

        /*allocates UArray if nonzero dimensions reported*/
        if (new_UA2->width == 0 || new_UA2->height == 0) {
                new_UA2->UArr = NULL;
        } else {
                new_UA2->UArr = UArray_new(new_UA2->width * new_UA2->height,
                                           bytes);
        }

        return new_UA2;
}


/*
 * UArray2_height
 * Description: Takes a UArray2 and returns the "height" (number of rows) of
 *              the UArray2.
 * Parameters:  T UArray2 -- the UArray2 whose height we want
 * Return:      An integer that is the height (number of rows) of the UArray2
 * Expects:     UArray2 to be non-NULL
 * Notes:
 *      - Will CRE if UArray is NULL
 */
int UArray2_height(UArray2_T UArray2)
{
        assert(UArray2 != NULL);
        return UArray2->height;
}


/*
 * UArray2_width
 * Description: Takes a UArray2 and returns the "width" (number of columns) of 
 *              the UArray2.
 * Parameters:  T UArray2 -- the UArray2 whose width we want
 * Return:      An integer that is the width (number of columns) of the UArray2
 * Expects:     UArray2 to not be NULL
 * Notes:
 *      - Will CRE if UArray is NULL
 */
int UArray2_width(UArray2_T UArray2)
{
        assert(UArray2 != NULL);
        return UArray2->width;
}


/*
 * UArray2_size
 * Description: Takes a UArray2 and returns the "size" (number of bytes)
 *              allocated for each element in the UArray2.
 * Parameters:  T UArray2 -- the UArray2 whose element size we want
 * Return:      An integer that is the size (number of bytes) allocated for
 *              each element in the UArray2
 * Expects:     UArray2 to not be NULL
 * Notes:
 *      - Will CRE if UArray2 is NULL
 */
int UArray2_size(UArray2_T UArray2)
{
        assert(UArray2 != NULL);
        return UArray2->size;
}


/*
 * UArray2_at
 * Description: Takes a UArray2 and returns a pointer to the slot in the
 *              UArray at the given coordinates.
 * Parameters:  T UArray2 -- the UArray2 to search
 *              int col -- the column of the slot
 *              int row -- the row of the slot
 * Return:      A pointer to the slot at index col x row
 * Expects:     - UArray2 to not be NULL
 *              - col and row to be within the dimensions of the array
 * Notes:
 *      - Will CRE if UArray2 is NULL
 *      - Will also CRE if invalid coordinates are provided (see Expects)
 */
void *UArray2_at(UArray2_T UArray2, int col, int row)
{
        assert(UArray2 != NULL);
        //assert(inRange(UArray2, col, row));
        int index = UArray2_width(UArray2) * row + col;
        return UArray_at(UArray2->UArr, index);
}


/*
 * UArray2_map_col_major
 * Description: Takes a UArray2 and applies a function to each element in
 *              column-major order.
 * Parameters:  T UArray2 -- the UArray2 to modify
 *              void apply() -- the function to apply to each element
 *              void *closure -- closure argument to gain info about UArray2
 * Return:      None
 * Expects:     - Supplied pointers to be non-NULL
 *              - apply() to be of correct function signature
 * Notes:
 *      - Will CRE if any of the supplied pointers are NULL
 */
extern void UArray2_map_col_major(UArray2_T UArray2,
                                  void apply(int col, int row, UArray2_T UArr,
                                             void *ptr, void *cl),
                                  void *closure)
{
        /*screen against invalid arguments*/
        assert(UArray2 != NULL);
        assert(apply != NULL);
        assert(closure != NULL);
        if (UArray2->UArr == NULL) {
                return;
        }

        /*applying function to elements in column-major order*/
        for (int i = 0; i < UArray2->width; i++) {
                for (int j = 0; j < UArray2->height; j++) {
                        void *ptr = UArray2_at(UArray2, i, j);
                        apply(i, j, UArray2, ptr, closure);
                }
        }
}


/*
 * UArray2_map_row_major
 * Description: Takes a UArray2 and applies a function to each element in 
 *              row-major order
 * Parameters:  T UArray2 -- the UArray2 to modify
 *              void apply -- the function to apply to each element
 *              void *closure -- closure argument to gain info about UArray2
 * Return:      None
 * Expects:    - Supplied pointers to be non-NULL
 *             - apply() to be of correct function signature
 * Notes:
 *      - Will CRE if any of the supplied pointers are NULL
 */
extern void UArray2_map_row_major(UArray2_T UArray2,
                                  void apply(int col, int row, UArray2_T UArr,
                                             void *ptr, void *cl),
                                  void *closure)
{
        /*screen against invalid arguments*/
        assert(UArray2 != NULL);
        assert(apply != NULL);
        assert(closure != NULL);
        if (UArray2->UArr == NULL) {
                return;
        }

        /*applying function to elements in row-major order*/
        for (int j = 0; j < UArray2->height; j++) {
                for (int i = 0; i < UArray2->width; i++) {
                        void *ptr = UArray2_at(UArray2, i, j);
                        apply(i, j, UArray2, ptr, closure);
                }
        }
}


/*
 * UArray2_free
 * Description: Takes a UArray2 and deallocates it from memory. Sets UArray2
 *              to NULL.
 * Parameters:  T UArray2 -- the UArray2 to free
 * Return:      None
 * Expects:     UArray2 to not be NULL
 * Notes:
 *      - Will CRE if UArray2 is NULL
 */
void UArray2_free(UArray2_T *UArray2)
{
        /*screening against NULL UArray2*/
        assert(UArray2 != NULL);
        assert(*UArray2 != NULL);

        /*freeing UArray if it exists*/
        if ((*UArray2)->UArr != NULL) {
                UArray_free(&((*UArray2)->UArr));
        }

        FREE(*UArray2);
        *UArray2 = NULL;
}


/*
 * inRange
 * Description: Takes in coordinates for an element in the 2D array and returns
 *              true if they are valid and false otherwise.
 * Parameters:  T UArray2 -- the UArray2 check coordinates of
 *              int col -- column coordinate to check
 *              int row -- row coordinate to check
 * Return:      A Boolean indicating if the coordinates are in range or not
 * Expects:     UArray2 to not be NULL
 * Notes:
 *      - Will CRE if UArray2 is NULL
 */
// extern bool inRange(UArray2_T UArray2, int col, int row)
// {
//         assert(UArray2 != NULL);
//         bool colInRange = (col >= 0 && col < UArray2->width);
//         bool rowInRange = (row >= 0 && row < UArray2->height);
//         return colInRange && rowInRange;
// }
