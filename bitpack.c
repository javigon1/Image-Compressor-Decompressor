#include "bitpack.h"


#define MAX_BITS 64

static inline uint64_t leftShiftUnsigned(uint64_t n, unsigned width)
{
        /* DO WE NEED TO ASSERT FOR WIDTH != 0??? */
        assert(width <= MAX_BITS);
        if (width < MAX_BITS) {
                n = n << width;
        } else {
                n = 0;
        }
        return n;
}

static inline uint64_t rightShiftUnsigned(uint64_t n, unsigned width)
{
        assert(width <= MAX_BITS);
        if (width < MAX_BITS) {
                n = n >> width;
        } else {
                n = 0;
        }
        return n;
}

static inline int64_t leftShiftSigned(int64_t n, unsigned width)
{
        assert(width <= MAX_BITS);
        if (width < MAX_BITS) {
                n = n << width;
        } else {
                n = 0;
        }
        return n;
}

static inline int64_t rightShiftSigned(int64_t n, unsigned width)
{
        assert(width <= MAX_BITS);
        if (width < MAX_BITS) {
                n = n >> width;
        } else {
                n = 0;
        }
        return n;
}


Except_T Bitpack_Overflow = { "Overflow packing bits" };


bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= MAX_BITS);
        uint64_t m = n;
        m = leftShiftUnsigned(m, MAX_BITS - width);
        m = rightShiftUnsigned(m, MAX_BITS - width);
        return (m == n);
}



bool Bitpack_fitss( int64_t n, unsigned width)
{
        assert(width <= MAX_BITS);
        int64_t m = n;
        m = leftShiftSigned(m, MAX_BITS - width);
        m = rightShiftSigned(m, MAX_BITS - width);
        return (m == n);
}



uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        if (width == 0) return 0;

        /* all bits set to 1 */
        uint64_t mask = ~0;
        /* shift to the left to only get the number of 1s == width */
        mask = leftShiftUnsigned(mask, MAX_BITS - width);
        /* shift back to position */
        mask = rightShiftUnsigned(mask, MAX_BITS - width - lsb);
        /* & both uint64_t's */
        uint64_t extracted_field = mask & word;
        /* shift result so that lsb is right-most bit */
        extracted_field = rightShiftUnsigned(extracted_field, lsb);

        return extracted_field;
}



int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        if (width == 0) return 0;

        int64_t signed_word = word;

        int64_t mask = signed_word < 0 ? ~0 : 0;

        mask = mask >> (MAX_BITS - width) << (lsb);

        int64_t extracted_field = mask & word;

        extracted_field =  extracted_field >> lsb;

        return extracted_field;

}



uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{
        (void)word;
        (void)width;
        (void)lsb;
        (void)value;
        uint64_t n = 2;
        return n;
}



uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
{
        (void)word;
        (void)width;
        (void)lsb;
        (void)value;
        uint64_t n = 2;
        return n;
}