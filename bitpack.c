#include "bitpack.h"
#include "calculations.h"


#define MAX_BITS 64


Except_T Bitpack_Overflow = { "Overflow packing bits" };


static inline uint64_t leftShift(uint64_t word, unsigned width)
{
        /* DO WE NEED TO ASSERT FOR WIDTH != 0??? */
        assert(width <= MAX_BITS);
        if (width < MAX_BITS) {
                word = word << width;
        } else {
                word = 0;
        }
        return word;
}

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


static inline int64_t rightShiftSigned(int64_t word, unsigned width)
{
        assert(width <= MAX_BITS);

        if (width < MAX_BITS) {
                word = word >> width;
        } else if (word >= 0) {
                word = 0;
        } else {
                word = ~0;       
        }

        return word;
}


bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        // if (width == 0){
        //         return false;
        // }
        if ((uint64_t)((1 << width) - 1) >= n){
                return true;
        }
        return false;
}



bool Bitpack_fitss(int64_t n, unsigned width)
{
        int64_t high = ((int64_t)(1 << width) / 2) - 1;
        int64_t low = (-1 * high) - 1;

        if (n >= low && n <= high){
                return true;
        }
        return false;
}


//LSB IS THE OFFSET -- NOT INCLUDED IN THE EXTRACTED BIT FIELD
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        if (width == 0) return 0;

        uint64_t mask = ~0;

        mask = leftShift(mask, MAX_BITS - width);
        mask = rightShiftUnsigned(mask, MAX_BITS - width - lsb);

        uint64_t extracted_field = (word & mask);
        extracted_field = rightShiftUnsigned(extracted_field, lsb);
        
        return extracted_field;
}



int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        if (width == 0) return 0;

        int64_t mask = leftShift(1, width);
        mask -= 1;
        mask = leftShift(mask, lsb);

        int64_t extracted_field = (word & mask);
        extracted_field = leftShift(extracted_field, lsb);

        //check if MSB isn't zero (number is negative)
        if ((extracted_field & leftShift(1, (width - 1))) != 0) {
                //flip bits more significant than width (making the returned 
                //number negative)
                extracted_field |= ~(leftShift(1, width) - 1);
        }

        return extracted_field;
}



uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

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



uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value)
{       
        int64_t mask = leftShift(1, width) - 1;
        mask = leftShift(mask, lsb);
        mask = ~mask;

        int64_t new_word = word;
        new_word = new_word & mask;

        int64_t val_mask = leftShift(1, width) - 1;

        int64_t value_field = value & val_mask;

        value_field = leftShift(value_field, lsb);

        new_word = new_word | value_field;

        if ((new_word & leftShift(1, 63)) != 0)
        {
                uint64_t sign_extension = leftShift(~0, (width + lsb));
                new_word |= sign_extension;
        }

        return new_word;
}