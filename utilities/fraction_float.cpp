#include "fraction_float.h"
#include <stdio.h>
#include <math.h>

using namespace rd;

fraction_float::fraction_float(signed long long numerator,signed long long denominator)
{
    m_numerator = numerator;
    m_denominator = denominator;
	reduction();
    if (m_denominator < 0) {
        m_denominator *= -1;
        m_numerator *= -1;
	}
}

fraction_float::~fraction_float(void)
{
}

void fraction_float::print()
{
    printf("~=% .15f	exact : %+I64i/%I64i\n",(double)m_numerator / (double)m_denominator,m_numerator,m_denominator);
}

fraction_float fraction_float::abs(const fraction_float src)
{
    fraction_float res;
    res.m_numerator = (src.m_numerator >= 0 ? src.m_numerator : src.m_numerator * -1);
    return res;
}

signed long long fraction_float::abs(const signed long long src)
{
    return src >= 0 ? src : src * -1;
}

unsigned long long fraction_float::gcd(unsigned long long x,unsigned long long y)
{
	if (0 == x) return y;
	if (0 == y) return x;
    unsigned long long *max = (x > y ? &x : &y);
    unsigned long long *min = (max == &x ? &y : &x);
    unsigned long long remainder = 1;
    while (remainder != 0) {
        remainder = *max % *min;
        if (remainder) {
            *max = *min;
            *min = remainder;
		}
	}
    return *min;
}

void fraction_float::reduction()
{
    unsigned long long div = gcd(abs(m_numerator),abs(m_denominator));
    m_numerator /= (signed long long)div;
    m_denominator /= (signed long long)div;
}

unsigned long long fraction_float::lcm(unsigned long long x,unsigned long long y,unsigned long long &x_multi,unsigned long long &y_multi)
{
	if (0 == x || 0 == y) return 0;
    unsigned long long *max = (x > y ? &x : &y);
    unsigned long long *min = (max == &x ? &y : &x);
	unsigned long long j = 1;
    for (unsigned long long i = *max;;i += *max) {
		if (0 == i % x && 0 == i % y) {
            unsigned long long max_multi = j;
            unsigned long long min_multi = i / *min;
            x_multi = (max == &x ? max_multi : min_multi);
            y_multi = (max == &y ? max_multi : min_multi);
			return i;
		}
		j ++;
	}
}

fraction_float fraction_float::operator + ()
{
	return *this;
}

fraction_float fraction_float::operator - ()
{
    fraction_float res;
    res.m_numerator = -m_numerator;
    res.m_denominator = m_denominator;
    return res;
}

fraction_float fraction_float::operator + (const fraction_float add_end)
{
    fraction_float res;
    unsigned long long this_multi = 1,add_end_multi = 1;
    res.m_denominator = lcm(m_denominator,add_end.m_denominator,this_multi,add_end_multi);
    res.m_numerator = m_numerator * this_multi + add_end.m_numerator * add_end_multi;
    res.reduction();
    return res;
}

fraction_float fraction_float::operator - (const fraction_float minu_end)
{
    fraction_float res = minu_end;
    res.m_numerator *= -1;
    return (*this + res);
}

fraction_float fraction_float::operator * (const fraction_float multiplier)
{
    fraction_float res;
    res.m_numerator = m_numerator * multiplier.m_numerator;
    res.m_denominator = m_denominator * multiplier.m_denominator;
    res.reduction();
    return res;
}

fraction_float fraction_float::operator = (const fraction_float src)
{
    m_numerator = src.m_numerator;
    m_denominator = src.m_denominator;
	return *this;
}

fraction_float fraction_float::operator += (const fraction_float add_end)
{
    *this = *this + add_end;
	return *this;
}

fraction_float fraction_float::operator -= (const fraction_float minu_end)
{
    *this = *this - minu_end;
	return *this;
}

fraction_float fraction_float::operator *= (const fraction_float multiplier)
{
    *this = *this * multiplier;
	return *this;
}

fraction_float fraction_float::operator ++ ()
{
    *this = *this + fraction_float(1,1);
	return *this;
}

fraction_float fraction_float::operator ++ (int)
{
    fraction_float res = *this;
	++(*this);
    return res;
}

fraction_float fraction_float::operator -- ()
{
    *this = *this - fraction_float(1,1);
	return *this;
}

fraction_float fraction_float::operator -- (int)
{
    fraction_float res = *this;
	--(*this);
    return res;
}

bool fraction_float::operator == (const fraction_float compare)
{
    return (m_numerator == compare.m_numerator && m_denominator == compare.m_denominator);
}

bool fraction_float::operator >  (const fraction_float compare)
{
    if (m_numerator >= 0 && compare.m_numerator < 0) return true;
    if (m_numerator <0 && compare.m_numerator >= 0) return false;

    unsigned long long this_numerator = 0,compare_numerator = 0;
    unsigned long long this_multi = 1,compare_multi = 1;
    lcm(m_denominator,compare.m_denominator,this_multi,compare_multi);

    this_numerator = m_numerator * this_multi;
    compare_numerator = compare.m_numerator * compare_multi;
    return (this_numerator > compare_numerator);
}

bool fraction_float::operator <  (const fraction_float compare)
{
    return !(*this == compare || *this > compare);
}

bool fraction_float::operator >= (const fraction_float compare)
{
    return *this == compare || *this > compare;
}

bool fraction_float::operator <= (const fraction_float compare)
{
    return !(*this > compare);
}

bool fraction_float::operator != (const fraction_float compare)
{
    return !(*this == compare);
}
