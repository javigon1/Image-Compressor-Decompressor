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
        (void)word;
        (void)width;
        (void)lsb;
        uint64_t n = 2;
        return n;
}



int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        (void)word;
        (void)width;
        (void)lsb;
        uint64_t n = 2;
        return n;
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