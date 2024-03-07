#include "bitpack.h"
#include "calculations.h"


#define MAX_BITS 64


Except_T Bitpack_Overflow = {"Overflow packing bits"};

/*
 * leftShift
 * Description: Takes a uint64_t and performs a left shift operation on it of
 *              magnitude width
 * Parameters:  uint64_t word - the value to shift
 *              unsigned width - how many bits to shift the word
 * Return:      a uint64_t resulting from shifting the original word
 * Expects:     width to be <= MAXBITS
 * Notes:
 *      - returns zero if shifted by MAXBITS bits
 *      - returns the original word if shifted by more MAXBITS bits
 */
static inline uint64_t leftShift(uint64_t word, unsigned width)
{
        assert(width <= MAX_BITS);

        if (width < MAX_BITS) {
                /* shift to the left by the amount wanted */
                word = word << width;
        } else {
                /* if shift is 64 then the word "disappears"*/
                word = 0;
        }
        return word;
}

/*
 * rightShiftUnsigned
 * Description: Takes a uint64_t and performs a right shift operation on it of
 *              magnitude width
 * Parameters:  uint64_t word - the value to shift
 *              unsigned width - how many bits to shift the word
 * Return:      a uint64_t resulting from shifting the original word
 * Expects:     width to be <= MAXBITS
 * Notes:
 *      - returns zero if shifted by MAXBITS bits
 *      - returns the original word if shifted by more MAXBITS bits
 */
static inline uint64_t rightShiftUnsigned(uint64_t word, unsigned width)
{
        assert(width <= MAX_BITS);

        if (width < MAX_BITS) {
                word = word >> width;
        } else {
                word = 0;
        }
        return word;
}

/*
 * rightShiftSigned
 * Description: Takes an int64_t and performs a right shift operation on it of
 *              magnitude width
 * Parameters:  int64_t word - the value to shift
 *              unsigned width - how many bits to shift the word
 * Return:      an int64_t resulting from shifting the original word
 * Expects:     width to be <= MAXBITS
 * Notes:
 *      - returns zero if shifted by MAXBITS bits
 *      Results in a cre if width is larger than MAXBITS
 */
static inline int64_t rightShiftSigned(int64_t word, unsigned width)
{
        assert(width <= MAX_BITS);

        if (width < MAX_BITS) {
                word = word >> width;
        /* check if the value of the word is positive */
        } else if (word >= 0) {
                word = 0;
        } else {
        /* if negative set word to all 1s */
                word = ~0;
        }
        return word;
}

/*
 * Bitpack_fitsu
 * Description: Takes a uint64_t and determines whether or not it can be
 *              represented in "width" bits
 * Parameters:  uint64_t n - the value (unsigned)
 *              unsigned width - num of bits we want to represent the value in
 * Return:      true if we can represent "n" in "width" bits, false if not
 * Expects:     width to be <= MAXBITS
 * Notes:       Results in a cre if width is larger than MAXBITS
 *
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= MAX_BITS);

        /* the math for unsigned is 2^n - 1 and 1 shift left replicates "*2" */
        if ((uint64_t)((1 << width) - 1) >= n) {
                return true;
        }
        return false;
}

/*
 * Bitpack_fitss
 * Description: Takes an int64_t and determines whether or not it can be
 *              represented in "width" bits
 * Parameters:  int64_t n - the value (signed)
 *              unsigned width - num of bits we want to represent the value in
 * Return:      true if we can represent "n" in "width" bits, false if not
 * Expects:     width to be <= MAXBITS
 * Notes:       Results in a cre if width is larger than MAXBITS
 *
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width <= MAX_BITS);

        /* the math for signed is 2^(n-1) - 1 - hence the "/ 2" */
        int64_t high = ((int64_t)(1 << width) / 2) - 1;
        /* the math for the lowest range for singed is -2^(n-1) */
        int64_t low = (-1 * high) - 1;

        if (n >= low && n <= high) {
                return true;
        }
        return false;
}

/*
 * Bitpack_getu
 * Description: Takes a uint64_t and returns the value field of width "width"
 *              whose least significant bit is "lsb"
 * Parameters:  uint64_t n - the word to search (unsigned)
 *              unsigned width - the width of the field to extract
 *              unsigned lsb - the offset/least significant bit of the field
 * Return:      a uint64_t containing the extracted value
 * Expects:     width <= MAXBITS, width + lsb <= MAXBITS
 * Notes:       Results in a cre if width is larger than MAXBITS or if width + 
 *              least significant bit is larger than MAXBITS
 *
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);

        /* set the mask to 1*/
        uint64_t mask = ~0;

        /* end up with a mask of 1s in the position we want to get */
        mask = leftShift(mask, MAX_BITS - width);
        mask = rightShiftUnsigned(mask, MAX_BITS - width - lsb);

        /* combine both the maks and the word to get the value at that point */
        uint64_t extracted_field = (word & mask);
        extracted_field = rightShiftUnsigned(extracted_field, lsb);

        return extracted_field;
}

/*
 * Bitpack_gets
 * Description: Takes a uint64_t and returns the value field of width "width"
 *              whose least significant bit is "lsb"
 * Parameters:  uint64_t n - the word to search (signed)
 *              unsigned width - the width of the field to extract
 *              unsigned lsb - the offset/least significant bit of the field
 * Return:      a uint64_t containing the extracted value
 * Expects:     width <= MAXBITS, width + lsb <= MAXBITS
 * Notes:       Results in a cre if width is larger than MAXBITS or if width +
 *              least significant bit is larger than MAXBITS
 *
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);

        int64_t mask = leftShift(1, width);
        mask -= 1;
        mask = leftShift(mask, lsb);

        int64_t extracted_field = (word & mask);

        extracted_field = rightShiftSigned(extracted_field, lsb);

        // check if MSB isn't zero (number is negative)
        if ((extracted_field & leftShift(1, (width - 1))) != 0) {
                // flip bits more significant than width (making the returned
                // number negative)
                extracted_field |= ~(leftShift(1, width) - 1);
        }

        return extracted_field;
}

/*
 * Bitpack_newu
 * Description: Take a uint64_t and returns a new uint64_t with the value field
 *              "value" of width "width" replacing the field of the same width
 *              at least significant bit "lsb"
 * Parameters:  uint64_t n - the word to modify (unsigned)
 *              unsigned width - the width of the field to add
 *              unsigned lsb - the offset/least significant bit of the field
 *              uint64_t value - the value field to add (unsigned)
 * Return:      a uint64_t containing the extracted value
 * Expects:     width <= MAXBITS, width + lsb <= MAXBITS
 * Notes:       Results in a cre if width is larger than MAXBITS or if width +
 *              least significant bit is larger than MAXBITS
 *
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);

        /* Check if value fits within the specified width */
        if (!Bitpack_fitsu(value, width)) RAISE(Bitpack_Overflow);

        uint64_t mask = leftShift(1, width) - 1;
       
        mask = leftShift(mask, lsb);
       
        mask = ~mask;
       
        uint64_t new_word = word;
        
        new_word = new_word & mask;

        uint64_t val_mask = leftShift(1, width) - 1;
      
        uint64_t value_field = value & val_mask;
       
        value_field = leftShift(value_field, lsb);
        
        new_word = new_word | value_field;
       

        return new_word;
}

/*
 * Bitpack_news
 * Description: Take a uint64_t and returns a new uint64_t with the value field
 *              "value" of width "width" replacing the field of the same width
 *              at least significant bit "lsb"
 * Parameters:  uint64_t n - the word to modify (unsigned)
 *              unsigned width - the width of the field to add
 *              unsigned lsb - the offset/least significant bit of the field
 *              uint64_t value - the value field to add (signed)
 * Return:      a uint64_t containing the extracted value
 * Expects:     width <= MAXBITS, width + lsb <= MAXBITS
 * Notes:       Results in a cre if width is larger than MAXBITS or if width +
 *              least significant bit is larger than MAXBITS
 *
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{
        assert(width <= MAX_BITS);
        assert(width + lsb <= MAX_BITS);

        /* Check if value fits within the specified width */
        if (!Bitpack_fitss(value, width)) RAISE(Bitpack_Overflow);

        int64_t mask = leftShift(1, width) - 1;
       
        mask = leftShift(mask, lsb);
       
        mask = ~mask;
      
        uint64_t new_word = word;
       
        new_word = new_word & mask;

        int64_t val_mask = leftShift(1, width) - 1;
       
        int64_t value_field = value & val_mask;

        value_field = leftShift(value_field, lsb);
       
        new_word = new_word | value_field;

        return new_word;
}