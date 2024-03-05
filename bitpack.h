#ifndef BITPACK_INCLUDED
#define BITPACK_INCLUDED


#include <stdbool.h>
#include <stdint.h>
#include "assert.h"
#include "except.h"


extern Except_T Bitpack_Overflow;


void print_uint64_binary(uint64_t num);
void print_int64_binary(int64_t num);

bool Bitpack_fitsu(uint64_t n, unsigned width);
bool Bitpack_fitss( int64_t n, unsigned width);

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value);
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value);


#endif
