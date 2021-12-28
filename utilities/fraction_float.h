#ifndef FRACTION_FLOAT_H
#define FRACTION_FLOAT_H

#include "rd.h"

namespace rd {

class API fraction_float
{
public:
    fraction_float(signed long long numerator = 0,signed long long denominator = 1);
    ~fraction_float(void);
public:
    void print();
    fraction_float abs(const fraction_float src);
    signed long long abs(const signed long long src);
    fraction_float operator + ();
    fraction_float operator - ();
    fraction_float operator + (const fraction_float add_end);
    fraction_float operator - (const fraction_float minu_end);
    fraction_float operator * (const fraction_float multiplier);
    fraction_float operator = (const fraction_float src);
    fraction_float operator += (const fraction_float add_end);
    fraction_float operator -= (const fraction_float minu_end);
    fraction_float operator *= (const fraction_float multiplier);
    fraction_float operator ++ ();
    fraction_float operator ++ (int);
    fraction_float operator -- ();
    fraction_float operator -- (int);
    bool operator == (const fraction_float compare);
    bool operator >  (const fraction_float compare);
    bool operator <  (const fraction_float compare);
    bool operator >= (const fraction_float compare);
    bool operator <= (const fraction_float compare);
    bool operator != (const fraction_float compare);
private:
	unsigned long long gcd(unsigned long long x,unsigned long long y);
    unsigned long long lcm(unsigned long long x,unsigned long long y,unsigned long long &x_multi,unsigned long long &y_multi);
	void reduction();
private:
    signed long long m_numerator;
    signed long long m_denominator;
};

} //namespace rd

#endif // FRACTION_FLOAT_H
