#ifndef MATH_H
#define MATH_H


#define MAX(i1, i2) (i1 > i2 ? i1 : i2)
#define MIN(i1, i2) (i1 < i2 ? i1 : i2)


void divMod(unsigned int num, unsigned int divisor, unsigned int *quotient, unsigned int *remainder);

void divModLL(unsigned long long num, unsigned long long divisor, unsigned long long *quotient,
              unsigned long long *remainder);

unsigned int numDigits(unsigned int val, unsigned int base);

unsigned long long numDigitsLL(unsigned long long val, unsigned long long base);


#endif //MATH_H
