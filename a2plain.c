/*******************************************************************************
 *
 *                              a2plain.c
 *
 *     Assignment: locality
 *     Authors:  Luciano Galvani and Kieran Fergus 
 *     Date:     2/22/24
 *
 *     This file contains an implementation of the A2Methods interface that
 *     creates a method suite to enable a UArray2 to act as a member A2Methods 
 *     under its operations.
 ******************************************************************************/

#include <string.h>
#include <a2plain.h>
#include "uarray2.h"

#define BLOCKSIZE 1


/*
 * A2Methods_UArray2 new
 * Description: Builds a 2-dimensional A2 UArray of the given dimensions,  
 *              reserving the indicated number of bytes for each element.
 * Parameters:  int col -- number of columns in the array
 *              int row -- number of rows in the array
 *              int bytes -- number of bytes allocated in memory for each
 *                           element
 * Return:      An empty "col" x "row" A2 UArray able to store elements of size
 *              "bytes"
 * Expects:     - col and row to be non-negative
 *              - bytes to be positive
 * Notes:
 *      - Will CRE if col or row is negative or if bytes is non-positive
 *      - Will also CRE if memory-related errors occur in allocating the array
 *      - Resulting UArray2 must be freed later using the Bit2_free function
 */
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, size);
}


/*
 * A2Methods_UArray2 new_with_blocksize
 * Description: Builds a 2-dimensional A2 UArray of the given dimensions,  
 *              reserving the indicated number of bytes for each element.
 *              Ignores blocksize parameter.
 * Parameters:  int col -- number of columns in the array
 *              int row -- number of rows in the array
 *              int bytes -- number of bytes allocated in memory for each
 *                           element
 *              int blocksize -- a number for blocksize to be ignored
 * Return:      An empty "col" x "row" A2 UArray able to store elements of size
 *              "bytes"
 * Expects:     - col and row to be non-negative
 *              - bytes to be positive
 * Notes:
 *      - Will CRE if col or row is negative or if bytes is non-positive
 *      - Will also CRE if memory-related errors occur in allocating the array
 *      - Resulting UArray2 must be freed later using the Bit2_free function
 */
static A2Methods_UArray2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        (void) blocksize;
        return UArray2_new(width, height, size);
}


/*apply function template*/
typedef void UArray2_applyfun2(int col, int row, UArray2_T UArr, void *ptr, 
                              void *cl);

/******************************************************************************
 * Already Implemented -  maps apply function in row-major order              *
 ******************************************************************************/
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_row_major(uarray2, (UArray2_applyfun*)apply, cl);
}


/******************************************************************************
 * Already Implemented -  maps apply function in column-major order           *
 ******************************************************************************/
static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_col_major(uarray2, (UArray2_applyfun*)apply, cl);
}


/*compacts closure for small apply functions*/
struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void                    *cl;
};

/******************************************************************************
 * Already Implemented -  calls an apply function for given coordinate        *
 ******************************************************************************/
static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl);
}


/******************************************************************************
 * Already Implemented -  maps  small apply function in row-major order       *
 ******************************************************************************/
static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}


/******************************************************************************
 * Already Implemented -  maps  small apply function in column-major order    *
 ******************************************************************************/
static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}



/******************************************************************************/
/************************* REMAINING FUNCTIONS ********************************/    
/******************************************************************************/

/*
 * free
 * Description: Takes an A2 UArray2 and deallocates it from memory.
 *              Sets pointer to NULL.
 * Parameters:  A2Methods_UArray2 a2 -- the UArray2 to free
 * Return:      None
 * Expects:     A2 UArray2 to not be NULL
 * Notes:
 *      - Will CRE if A2 UArray2 is NULL
 */
static void a2free(A2Methods_UArray2 *a2)
{
        UArray2_free((UArray2_T *)a2);
}


static int width(A2Methods_UArray2 a2)
{
        return UArray2_width(a2);
}


/*
 * height
 * Description: Takes an A2 UArray2 and returns the "height" (number of rows)
 *              of the UArray2.
 * Parameters:  A2Methods_UArray2 a2 -- the A2 UArray2 whose height we want
 * Return:      An integer that is the height (number of rows) of the UArray2
 * Expects:     A2 UArray2 to be non-NULL
 * Notes:
 *      - Will CRE if UArray is NULL
 */
static int height(A2Methods_UArray2 a2)
{
        return UArray2_height(a2);
}


/*
 * size
 * Description: Takes an A2 UArray2 and returns the "size" (number of bytes)
 *              allocated for each element.
 * Parameters:  A2Methods_UArray2 a2 -- A2 UArray2 whose element size we want
 * Return:      An integer that is the size (number of bytes) allocated for
 *              each element in the UArray2
 * Expects:     A2 UArray2 to not be NULL
 * Notes:
 *      - Will CRE if UArray2 is NULL
 */
static int size(A2Methods_UArray2 a2)
{
        return UArray2_size(a2);
}


/*
 * blocksize
 * Description: Takes an A2 UArray2 and returns its "blocksize, "
 * Parameters:  A2Methods_UArray2 a2 -- A2 UArray2 whose element size we want
 * Return:      An integer that is the size (number of bytes) allocated for
 *              each element in the UArray2
 * Expects:     A2 UArray2 to not be NULL
 * Notes:
 *      - Will CRE if UArray2 is NULL
 */
static int blocksize(A2Methods_UArray2 a2)
{
        (void) a2;
        return BLOCKSIZE;
}


/*
 * at
 * Description: Takes an A2 UArray2 and returns a pointer to the slot in the
 *              array at the given coordinates.
 * Parameters:  A2Methods_UArray2 a2 -- the A2 UArray2 to search
 *              int col -- the column of the slot
 *              int row -- the row of the slot
 * Return:      A pointer to the slot at index col x row
 * Expects:     - A2 UArray2 to not be NULL
 *              - col and row to be within the dimensions of the array
 * Notes:
 *      - Will CRE if UArray2 is NULL
 *      - Will also CRE if invalid coordinates are provided (see Expects)
 */
static void *at(A2Methods_UArray2 a2, int col, int row)
{
        return UArray2_at(a2, col, row);
}



/******************************************************************************/
/***************************** FINISHING UP ***********************************/    
/******************************************************************************/

/*full method suite*/
    static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        blocksize,   /*blocksize*/
        at,
        map_row_major,
        map_col_major,
        NULL,          /*map_block_major*/
        map_row_major, /*map_default*/
        small_map_row_major,
        small_map_col_major,
        NULL,               /*small_map_block_major*/
        small_map_row_major /*small_map_default*/
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
