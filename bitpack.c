#include "bitpack.h"
#include "calculations.h"
<<<<<<< HEAD

#define MAX_BITS 64

Except_T Bitpack_Overflow = {"Overflow packing bits"};

void print_uint64_binary(uint64_t num)
{
        for (int i = 63; i >= 0; i--)
        {
                printf("%lu", (num >> i) & 1);
                if (i % 8 == 0)
                        printf(" "); // Print space every 8 bits for better readability
        }
        printf("\n");
}

void print_int64_binary(int64_t num)
{
        for (int i = 63; i >= 0; i--)
        {
                printf("%lu", (num >> i) & 1);
                if (i % 8 == 0)
                        printf(" "); // Print space every 8 bits for better readability
        }
        printf("\n");
}
=======


#define MAX_BITS 64


Except_T Bitpack_Overflow = { "Overflow packing bits" };

>>>>>>> origin/main

static inline uint64_t leftShift(uint64_t word, unsigned width)
{
        /* DO WE NEED TO ASSERT FOR WIDTH != 0??? */
        assert(width <= MAX_BITS);
<<<<<<< HEAD
        if (width < MAX_BITS)
        {
                word = word << width;
        }
        else
        {
=======
        if (width < MAX_BITS) {
                word = word << width;
        } else {
>>>>>>> origin/main
                word = 0;
        }
        return word;
}

static inline uint64_t rightShiftUnsigned(uint64_t word, unsigned width)
{
        assert(width <= MAX_BITS);
<<<<<<< HEAD
        if (width < MAX_BITS)
        {
                word = word >> width;
        }
        else
        {
=======
        if (width < MAX_BITS) {
                word = word >> width;
        } else {
>>>>>>> origin/main
                word = 0;
        }
        return word;
}

<<<<<<< HEAD
=======

>>>>>>> origin/main
static inline int64_t rightShiftSigned(int64_t word, unsigned width)
{
        assert(width <= MAX_BITS);

<<<<<<< HEAD
        if (width < MAX_BITS)
        {
                word = word >> width;
        }
        else if (word >= 0)
        {
                word = 0;
        }
        else
        {
                word = ~0;
=======
        if (width < MAX_BITS) {
                word = word >> width;
        } else if (word >= 0) {
                word = 0;
        } else {
                word = ~0;       
>>>>>>> origin/main
        }

        return word;
}

<<<<<<< HEAD
=======

>>>>>>> origin/main
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        // if (width == 0){
        //         return false;
        // }
<<<<<<< HEAD
        if ((uint64_t)((1 << width) - 1) >= n)
        {
=======
        if ((uint64_t)((1 << width) - 1) >= n){
>>>>>>> origin/main
                return true;
        }
        return false;
}

<<<<<<< HEAD
=======


>>>>>>> origin/main
bool Bitpack_fitss(int64_t n, unsigned width)
{
        int64_t high = ((int64_t)(1 << width) / 2) - 1;
        int64_t low = (-1 * high) - 1;

<<<<<<< HEAD
        if (n >= low && n <= high)
        {
=======
        if (n >= low && n <= high){
>>>>>>> origin/main
                return true;
        }
        return false;
}

<<<<<<< HEAD
// LSB IS THE OFFSET -- NOT INCLUDED IN THE EXTRACTED BIT FIELD
=======

//LSB IS THE OFFSET -- NOT INCLUDED IN THE EXTRACTED BIT FIELD
>>>>>>> origin/main
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

<<<<<<< HEAD
        if (width == 0)
                return 0;
=======
        if (width == 0) return 0;
>>>>>>> origin/main

        uint64_t mask = ~0;

        mask = leftShift(mask, MAX_BITS - width);
        mask = rightShiftUnsigned(mask, MAX_BITS - width - lsb);

        uint64_t extracted_field = (word & mask);
        extracted_field = rightShiftUnsigned(extracted_field, lsb);
<<<<<<< HEAD

=======
        
>>>>>>> origin/main
        return extracted_field;
}

int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

<<<<<<< HEAD
        if (width == 0)
                return 0;
=======
        if (width == 0) return 0;
>>>>>>> origin/main

        int64_t mask = leftShift(1, width);
        mask -= 1;
        mask = leftShift(mask, lsb);

        int64_t extracted_field = (word & mask);
        extracted_field = leftShift(extracted_field, lsb);

<<<<<<< HEAD
        // check if MSB isn't zero (number is negative)
        if ((extracted_field & leftShift(1, (width - 1))) != 0)
        {
                // flip bits more significant than width (making the returned
                // number negative)
=======
        //check if MSB isn't zero (number is negative)
        if ((extracted_field & leftShift(1, (width - 1))) != 0) {
                //flip bits more significant than width (making the returned 
                //number negative)
>>>>>>> origin/main
                extracted_field |= ~(leftShift(1, width) - 1);
        }

        return extracted_field;
}

<<<<<<< HEAD
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
=======


uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
>>>>>>> origin/main
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

<<<<<<< HEAD
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, int64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        if (!Bitpack_fitsu(value, width)) RAISE(Bitpack_Overflow);

        int64_t mask = leftShift(1, width) - 1;
        printf("mask:                   ");
        print_int64_binary(mask);
        mask = leftShift(mask, lsb);
        mask = ~mask;
        printf("mask:                   ");
        print_int64_binary(mask);

        uint64_t new_word = word;
        printf("new_word:               ");
        print_uint64_binary(new_word);
        new_word = new_word & mask;
        printf("new_word & mask:        ");
        print_uint64_binary(new_word);

        int64_t val_mask = leftShift(1, width) - 1;
        printf("value:                  ");
        print_int64_binary(value);
        int64_t value_field = value & val_mask;
        printf("value_field:            ");
        print_int64_binary(value_field);

        value_field = leftShift(value_field, lsb);
        printf("lsb << value_field:     ");

        print_int64_binary(value_field);

        new_word = new_word | value_field;
        printf("new_word | value_field: ");
        print_uint64_binary(new_word);

        // if ((new_word & leftShift(1, 63)) != 0)
        // {
        //         uint64_t sign_extension = leftShift(~0, (width + lsb));
        //         new_word |= sign_extension;
        // }
=======


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
>>>>>>> origin/main

        return new_word;
}