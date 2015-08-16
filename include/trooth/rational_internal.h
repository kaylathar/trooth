#ifndef RATIONAL_INTERNAL_H
#define RATIONAL_INTERNAL_H

struct TR_Rational
{
	TR_BigInt* numerator;
	TR_BigInt* denominator;
	TR_Environment* environment;
};

#endif
