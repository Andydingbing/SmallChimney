#ifndef GEN_INT_H
#define GEN_INT_H

#define MAX_DIGIT 255
#define MAX_BINARY_DIGIT 1024

#include "global_header.h"
#include <string.h>
#include <stddef.h>
#include <math.h>

// A general integer class
class API gen_int
{
public:
    gen_int(const char *in = nullptr)
    {
        ZERO_ARRAY(_num);
        _sign = '+';

        if (in == nullptr) {
            return;
        }

        _sign = (!(in[0] - '-') || !(in - '+') ? in[0] : '+');

        int start_pos = (!(in[0] - '-') || !(in - '+') ? 1 : 0);
        int num_idx = 0;

        for (int i = int(strlen(in) - 1);i >= start_pos;i --) {
            if (in[i] >= '0' && in[i] <= '9') {
                _num[num_idx] = in[i] - '0';
                num_idx ++;
            }
        }
    }

    gen_int(const gen_int &in) { *this = in; }

    short get_digit() const
    {
        short i = MAX_DIGIT;
        for (i = MAX_DIGIT;i >= 0;i --) {
            if (_num[i]) {
                break;
            }
        }
        i = i < 0 ? 0 : i;
        return i + 1;
    }

    gen_int abs(const gen_int& src)
    {
        gen_int res;

        for (short i = 0;i < MAX_DIGIT + 1;i ++) {
            res._num[i] = src._num[i];
        }
        res._sign = '+';
        return res;
    }

    gen_int operator + () { return *this; }

    gen_int operator - ()
    {
        gen_int res;

        for (short i = 0;i < MAX_DIGIT + 1;i ++) {
            res._num[i] = _num[i];
        }
        res._sign = ((0 == _sign - '+') ? '-' : '+');
        return res;
    }

    gen_int operator + (const gen_int add_end)
    {
        gen_int res;
        short carry = 0;
        short this_digit = this->get_digit();
        short add_end_digit = add_end.get_digit();
        short smaxDigit = this_digit > add_end_digit ? this_digit : add_end_digit;
        int i = 0;

        if (0 == (_sign - add_end._sign)) {
            for (i = 0;i < smaxDigit;i ++) {
                res._num[i] = _num[i] + add_end._num[i] + carry;
                if (res._num[i] >= 10) {
                    carry = 1;
                    res._num[i] -= 10;
                } else {
                    carry = 0;
                }
            }
            res._num[i] = carry;
        } else {
            gen_int *max = (abs(*this) >= abs(add_end) ? this : const_cast<gen_int *>(&add_end));
            gen_int *min = (max == this ? const_cast<gen_int *>(&add_end) : this);

            for (i = 0;i < smaxDigit;i ++) {
                max->_num[i] -= carry;
                if (max->_num[i] < 0) {
                    max->_num[i] = 9;
                    carry = 1;
                    res._num[i] = max->_num[i] - min->_num[i];
                    continue;
                }
                if (max->_num[i] < min->_num[i]) {
                    max->_num[i] += 10;
                    carry = 1;
                } else {
                    carry = 0;
                }
                res._num[i] = max->_num[i] - min->_num[i];
            }
            res._sign = ((0 == max->_sign - '+') ? '+' : '-');
        }
        return res;
    }

    gen_int operator - (const gen_int minu_end)
    {
        gen_int res = minu_end;
        res._sign = ((0 == res._sign - '+') ? '-' : '+');
        return (*this + res);
    }

    gen_int operator * (const gen_int multiplier)
    {
        gen_int res;
        short multi_digit = multiplier.get_digit();
        gen_int this_positive = abs(*this);
        gen_int multi_positive = abs(multiplier);

        for (short i = 0;i < multi_digit;i ++) {
            res += (this_positive * multi_positive._num[i]) << i;
        }
        res._sign = (0 == _sign - multiplier._sign ? '+' : '-');
        return res;
    }

    gen_int operator / (const int64_t divider)
    {
        gen_int this_copy = *this;
        gen_int res;
        uint64_t divider_abs = divider < 0 ? uint64_t(divider * -1) : uint64_t(divider);
        short div_digit = get_digit(divider_abs);
        short to = this_copy.get_digit() - 1,from = to - div_digit + 1;
        uint64_t div_end = this_copy._2ull(from,to);
        uint64_t remainder = 0;

        if (div_end < divider_abs) {
            from --;
            from = from < 0 ? 0 : from;
            div_end = this_copy._2ull(from,to);
        }
        for (short i = from;i >= 0;i --) {
            res._num[i] = short(div_end / divider_abs);
            remainder = div_end % divider_abs;
            for (short j = 0;j <= div_digit;j ++) {
                this_copy._num[from + j] = short(remainder % uint64_t(pow(10.0,j + 1)) / uint64_t(pow(10.0,j)));
            }
            from --;
            if (!this_copy._num[to]) {
                to --;
            }
            div_end = this_copy._2ull(from,to);
        }
        remainder *= 10;
        if (divider_abs * 5 < remainder) {
            res ++;
        }
        if ((0 == this_copy._sign - '+' && divider < 0) || (0 == this_copy._sign - '-' && divider > 0)) {
            res._sign = '-';
        }
        return res;
    }

    int64_t operator % (const int64_t divider)
    {
        gen_int this_copy = *this;
        gen_int Res;
        uint64_t divider_abs = divider < 0 ? uint64_t(divider * -1) : uint64_t(divider);
        short sDivDigit = get_digit(divider_abs);
        short to = this_copy.get_digit() - 1,from = to - sDivDigit + 1;
        uint64_t div_end = this_copy._2ull(from,to);
        uint64_t remainder = 0;

        if (div_end < divider_abs) {
            from --;
            from = from < 0 ? 0 : from;
            div_end = this_copy._2ull(from,to);
        }
        for (short i = from;i >= 0;i --) {
            Res._num[i] = short(div_end / divider_abs);
            remainder = div_end % divider_abs;
            for (short j = 0;j <= sDivDigit;j ++) {
                this_copy._num[from + j] = short(remainder % uint64_t(pow(10.0,j + 1)) / uint64_t(pow(10.0,j)));
            }
            from --;
            if (!this_copy._num[to]) {
                to --;
            }
            div_end = this_copy._2ull(from,to);
        }
        return remainder;
    }

    gen_int& operator = (const gen_int& src)
    {
        for (short i = 0;i < MAX_DIGIT + 1;i ++) {
            _num[i] = src._num[i];
        }
        _sign = src._sign;
        return *this;
    }

    gen_int operator += (const gen_int add_end) { *this = *this + add_end; return *this; }
    gen_int operator -= (const gen_int min_end) { *this = *this - min_end; return *this; }
    gen_int operator *= (const gen_int multipe) { *this = *this * multipe; return *this; }
    gen_int operator /= (const signed long long divider)
    {
        *this = *this / divider;
        return *this;
    }

    gen_int operator ++ ()
    {
        *this = *this + gen_int("1");
        return *this;
    }

    gen_int operator ++ (int)
    {
        gen_int res = *this;
        ++(*this);
        return res;
    }

    gen_int operator -- ()
    {
        *this = *this - gen_int("1");
        return *this;
    }

    gen_int operator -- (int)
    {
        gen_int res = *this;
        --(*this);
        return res;
    }

    bool operator == (const gen_int compare)
    {
        if (_sign - compare._sign) {
            return false;
        }

        for (short i = 0;i < MAX_DIGIT + 1;i ++) {
            if (_num[i] != compare._num[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator > (const gen_int compare)
    {
        if (_sign < compare._sign) {
            return true;
        }

        if (_sign > compare._sign) {
            return false;
        }

        for (short i = MAX_DIGIT;i >= 0;i --) {
            if ((0 == _num[i] && 0 == compare._num[i]) || (_num[i] == compare._num[i])) {
                continue;
            } else if (_num[i] > compare._num[i]) {
                return (0 == _sign - '+') ?  true : false;
            } else {
                return (0 == _sign - '+') ? false : true;
            }
        }
        return false;
    }

    bool operator <  (const gen_int compare) { return !(*this == compare || *this > compare); }
    bool operator >= (const gen_int compare) { return *this == compare || *this > compare; }
    bool operator <= (const gen_int compare) { return !(*this > compare); }
    bool operator != (const gen_int compare) { return !(*this == compare); }

    gen_int operator << (short digit)
    {
        if (digit < 0) {
            return (*this >> (digit * -1));
        }
        if (digit > 0) {
            for (short i = MAX_DIGIT;i >= digit;i --) {
                _num[i] = _num[i - digit];
            }
            for (short i = 0;i < digit;i ++) {
                _num[i] = 0;
            }
        }
        return *this;
    }

    gen_int operator >> (short digit)
    {
        if (digit < 0) {
            return (*this << (digit * -1));
        }
        if (digit > 0) {
            short i = 0;
            for (i = 0;i < MAX_DIGIT - digit;i ++) {
                _num[i] = _num[i + digit];
            }
            for (;i < MAX_DIGIT;i++) {
                _num[i] = 0;
            }
        }
        return *this;
    }

public:
    //directly to unsigned long long,regardless of where it is
    uint64_t _2ull(short from,short to)
    {
        short sDigit = to - from + 1;
        unsigned long long res = 0;

        for (short i = 0;i < sDigit;i ++) {
            res += uint64_t(_num[from + i]) * uint64_t(pow(10.0,i));
        }
        return res;
    }

    short odd_part() { return short(_num[0] % 2); }

    void _2bin_s(short *bin,short digit)
    {
        short res[MAX_BINARY_DIGIT] = {0};
        gen_int divider = abs(*this);
        gen_int div_res("1");
        short i = 0;
        short j = 0;

        while (gen_int("0") != div_res) {
            div_res = divider / 2;
            res[i] = divider.odd_part();
            divider = div_res;
            i ++;
        }
        if ((0 == _sign - '-') && (gen_int("0") != *this)) {
            short carry = 0;
            for (short i = 0;i < MAX_BINARY_DIGIT;i ++) {
                res[i] = (0 == res[i] ? 1 : 0);
            }
            carry = (1 == res[0] ? 1 : 0);
            res[0] = (1 == res[0] ? 0 : 1);
            for (short i = 1;i < MAX_BINARY_DIGIT;i ++) {
                res[i] += carry;
                if (2 == res[i]) {
                    res[i] = 0;
                    carry = 1;
                } else {
                    carry = 0;
                }
            }
        }
        for (j = 0;j < digit;j ++) {
            bin[j] = res[j];
        }
    }

    void _2ui64(unsigned int &high,unsigned int &low)
    {
        short bin[64] = {0};

        high = low = 0;
        _2bin_s(bin,64);

        for (short i = 0;i < 32;i ++) {
            low  += bin[i] << i;
            high += bin[32 + i] << i;
        }
    }

    gen_int operator * (const short multiplier)
    {
        gen_int res;
        char multi_sign = (multiplier >= 0 ? '+' : '-');
        short digit = get_digit();
        short carry = 0;
        short i = 0;

        for (i = 0;i < digit;i ++) {
            res._num[i] = _num[i] * multiplier + carry;
            carry = res._num[i] / 10;
            res._num[i] -= carry * 10;
        }
        res._num[i] = carry;
        res._sign = (0 == _sign - multi_sign ? '+' : '-');
        return res;
    }

    short get_digit(const unsigned long long src)
    {
        short j = 1;

        for (unsigned long long i = 10;;i *= 10) {
            if (0 == src / i)
                break;
            j ++;
        }
        return j;
    }

private:
    short _num[MAX_DIGIT + 1];
    char _sign;
};

#endif
