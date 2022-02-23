/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef UTILITIES_ALGO_MATH_HPP
#define UTILITIES_ALGO_MATH_HPP

#include "exception.hpp"
#include "traits.hpp"
#include <boost/math/constants/constants.hpp>

template<typename T,
template<typename _Tp = T,typename allocator_t = std::allocator<_Tp>> class containter_t>
T sum(const containter_t<T> &x)
{
    T x_sum = 0;
    typename containter_t<T>::const_iterator iter = x.cbegin();

    for (;iter != x.cend();++iter) {
        x_sum += *iter;
    }
    return x_sum;
}

double API round(double x,uint32_t digit);
SYM_INLINE double round(double x) { return round(x,0); }

template<typename T>
typename std::vector<T>::size_type min_idx(const std::vector<T> &x)
{
    size_t i = 0;
    size_t i_min = 0;
    T x_min = x[0];

    for (i = 0;i < x.size();i ++) {
        if (x_min > x[i]) {
            x_min = x[i];
            i_min = i;
        }
    }
    return i_min;
}


template<typename x_t,typename bound_t>
SYM_INLINE bool is_after(const x_t &x,const bound_t &lower_bound)
{ return (x >= lower_bound); }

template<typename x_t,typename bound_t>
SYM_INLINE bool is_before(const x_t &x,const bound_t &upper_bound)
{ return (x <= upper_bound); }

template<typename x_t,typename bound_t>
SYM_INLINE bool is_between(const x_t &x,const bound_t &lower_bound,const bound_t &upper_bound)
{ return (x >= lower_bound) && (x <= upper_bound); }

template<typename T>
SYM_INLINE void limit_after(const T &lower_bound,T &x)
{ x = x < lower_bound ? lower_bound : x; }

template<typename T>
SYM_INLINE void limit_before(const T &upper_bound,T &x)
{ x = x > upper_bound ? upper_bound : x; }

template<typename T>
SYM_INLINE void limit_between(const T &lower_bound,const T &upper_bound,T &x)
{ limit_after(lower_bound,x); limit_before(upper_bound,x); }

template<typename int_type>
SYM_INLINE boost::rational<int_type> pow(boost::rational<int_type> x,size_t y)
{ x.assign(int_type(std::pow(x.numerator(),y)),int_type(std::pow(x.denominator(),y))); return x; }

template<typename T_x,typename T_y>
SYM_INLINE void linear(T_x x1,T_y y1,T_x x2,T_y y2,T_x x,T_y &y)
{ y = T_y(double(x - x1) / double(x2 - x1) * double(y2 - y1) + y1); }

template<typename T>
SYM_INLINE T linear_quantify(const T star,const T step,const T value)
{ return uint64_t((value - star) / step) * step + star; }

template<typename T>
void discretept(T x,int64_t step,T &x1,T &x2)
{
    T x_min = T(int64_t((int64_t(x / (step * 1.0))) * step));

    if (x_min > x) {
        x1 = step < 0 ? x_min : (x_min - step);
    } else if (x_min < x) {
        x1 = step < 0 ? (x_min - step) : x_min;
    } else {
        x1 = x_min;
    }
    x2 = x1 + step;
}

// 1 / sqrt(x)
float SYM_INLINE inv_sqrt(float x);

template<typename ad_base_t,typename ad_t>
SYM_INLINE double ad_to_dBc(ad_base_t x,ad_t x1)
{ return double(10.0 * log10(double(x1) / double(x))); }

template<typename ad_base_t,typename ad_t,typename dBc_t>
SYM_INLINE void ad_to_dBc(ad_base_t x,ad_t x1,dBc_t &dbc)
{ dbc = dBc_t(10.0 * log10(double(x1) / double(x))); }

template<typename ad_t,typename dBc_t>
SYM_INLINE ad_t dBc_to_ad(ad_t x,dBc_t dBc)
{ return ad_t(pow(10.0,(-1.0 * double(dBc) / 10.0)) * x); }

template<typename T,typename data_type = float_traits<T>>
SYM_INLINE T magnitude(T i,T q,T full_scale)
{ return ad_to_dBc(full_scale,i * i + q * q); }

template<typename T,typename data_type = float_traits<T>>
SYM_INLINE T magnitude(T v,T full_scale)
{ return ad_to_dBc(full_scale,v); }

template<typename T,typename data_type = float_traits<T>>
SYM_INLINE T angle_to_radian(const T angle)
{ return angle / T(180.0) * boost::math::constants::pi<T>(); }

namespace descriptive_statistics {

template <typename T,typename data_type = float_traits<T>>
class descriptive_statistics_t {
public:
    static T mean(const std::vector<T> &x)
    {
        T x_ = T(0.0);
        for (size_t i = 0;i < x.size();i ++) {
            x_ += x[i];
        }
        return x_ / x.size();
    }

    static T var(const std::vector<T> &x)
    {
        T x_mean = mean(x);
        T variance = T(0.0);
        for (size_t i = 0;i < x.size();i ++) {
            variance += (x[i] - x_mean) * (x[i] - x_mean);
        }
        return variance / (x.size() - 1);
    }

    static T var(const std::vector<T> &x1,const std::vector<T> &x)
    {
        T variance = T(0.0);
        for (size_t i = 0;i < x1.size();i ++) {
            variance += (x1[i] - x[i]) * (x1[i] - x[i]);
        }
        return variance / (x.size() - 1);
    }
};

template<typename T>
SYM_INLINE T mean(const std::vector<T> &x)
{ return descriptive_statistics_t<T>::mean(x); }

template<typename T>
SYM_INLINE T var(const std::vector<T> &x)
{ return descriptive_statistics_t<T>::var(x); }

template<typename T>
SYM_INLINE T var(const std::vector<T> &x1,const std::vector<T> &x)
{ return descriptive_statistics_t<T>::var(x1,x); }

} // namespace descriptive_statistics


// sumxmul = a[0] * b[n] + a[1] * b[n - 1] + a[2] * b[n - 2] + ... + a[n] * b[0]
template<typename T,typename data_type = float_traits<T>>
class sumxmul_t {
public:
    // Unsafed overload
    static T sumxmul(typename std::vector<T>::const_iterator a,
                     typename std::vector<T>::const_iterator b,size_t n)
    {
        T y = T(0);

        for (size_t i = 0;i <= n;i ++) {
            y += a[i] * b[n - i];
        }
        return y;
    }

    static T sumxmul(const std::vector<T> &a,typename std::vector<T>::const_iterator iter_a,
                     const std::vector<T> &b,typename std::vector<T>::const_iterator iter_b,
                     size_t n)
    {
        if (iter_a - a.begin() + n >= a.size() || iter_b - b.begin() + n >= b.size()) {
            throw index_err(RD_THROW_SITE_INFO("Out of range"));
        }

        return sumxmul(iter_a,iter_b,n);
    }

    static T sumxmul(const std::vector<T> &a,const std::vector<T> &b,size_t n)
    { return sumxmul(a,a.begin(),b,b.begin(),n); }
};

template<typename T>
SYM_INLINE T sumxmul(typename std::vector<T>::const_iterator a,
                     typename std::vector<T>::const_iterator b,
                     size_t n) { return sumxmul_t<T>::sumxmul(a,b,n); }

template<typename T>
SYM_INLINE T sumxmul(const std::vector<T> &a,typename std::vector<T>::const_iterator iter_a,
                     const std::vector<T> &b,typename std::vector<T>::const_iterator iter_b,
                     size_t n) { return sumxmul_t<T>::sumxmul(a,iter_a,b,iter_b,n); }

template<typename T>
SYM_INLINE T sumxmul(const std::vector<T> &a,
                     const std::vector<T> &b,
                     size_t n) { return sumxmul_t<T>::sumxmul(a,b,n); }


// convolution and polynomial multiplication
template<typename T,typename data_type = float_traits<T>>
class conv_t {
public:
    static void conv(
            typename std::vector<T>::const_iterator iter_a_begin,size_t a_length,
            typename std::vector<T>::const_iterator iter_b_begin,size_t b_length,
            std::vector<T> &coef,typename std::vector<T>::iterator iter_coef)
    {
        typedef typename std::vector<T>::const_iterator iter_type;

        iter_type A;
        iter_type B;

        a_length = (a_length == 0 || a_length == 1) ? (a_length + 1) : a_length;
        b_length = (b_length == 0 || b_length == 1) ? (b_length + 1) : b_length;
        size_t m = a_length - 1;
        size_t M = 0;
        size_t n = b_length - 1;
        size_t N = 0;

        if (coef.size() - (iter_coef - coef.begin()) < m + n + 1) {
            size_t coef_begin = iter_coef - coef.begin();
            coef.clear();
            coef.assign(m + n + 1,T(0));
            iter_coef = coef.begin() + coef_begin;
        }

        if (m >= n) {
            A = iter_a_begin;
            M = m;
            B = iter_b_begin;
            N = n;
        } else {
            A = iter_b_begin;
            M = n;
            B = iter_a_begin;
            N = m;
        }

        for (size_t i = 0;i <= N;i ++) {
            iter_coef[i] = (sumxmul<data_type::data_t>(A,B,i));
        }
        for (size_t i = N + 1;i <= M;i ++) {
            iter_coef[i] = (sumxmul<data_type::data_t>(A + (i - N),B,N));
        }
        for (size_t i = M + 1;i <= M + N;i ++) {
            iter_coef[i] = (sumxmul<data_type::data_t>(A + (i - N),B + (i - M),N + M - i));
        }
    }

    static void conv(const std::vector<T> &a,size_t a_length,
                     const std::vector<T> &b,size_t b_length,
                     std::vector<T> &coef,typename std::vector<T>::iterator iter_coef)
    { return conv(a.begin(),a_length,b.begin(),b_length,coef,iter_coef); }

    static void conv(const std::vector<T> &a,size_t a_length,
                     const std::vector<T> &b,size_t b_length,
                     std::vector<T> &coef)
    { return conv(a.begin(),a_length,b.begin(),b_length,coef,coef.begin()); }

    static void conv(const std::vector<T> &a,
                     const std::vector<T> &b,
                     std::vector<T> &coef)
    { return conv(a.begin(),a.size(),b.begin(),b.size(),coef,coef.begin()); }
};

template<typename T>
SYM_INLINE T conv(typename std::vector<T>::const_iterator iter_a_begin,size_t a_length,
                  typename std::vector<T>::const_iterator iter_b_begin,size_t b_length,
                  std::vector<T> &coef,typename std::vector<T>::iterator iter_coef)
{ return conv_t<T>::conv(iter_a_begin,a_length,iter_b_begin,b_length,coef,iter_coef); }

template<typename T>
SYM_INLINE T conv(const std::vector<T> &a,size_t a_length,
                  const std::vector<T> &b,size_t b_length,
                  std::vector<T> &coef,typename std::vector<T>::iterator iter_coef)
{ return conv_t<T>::conv(a,a_length,b,b_length,coef,iter_coef); }

template<typename T>
SYM_INLINE T conv(const std::vector<T> &a,size_t a_length,
                  const std::vector<T> &b,size_t b_length,
                 std::vector<T> &coef)
{ return conv_t<T>::conv(a,a_length,b,b_length,coef); }

template<typename T>
SYM_INLINE T conv(const std::vector<T> &a,
                  const std::vector<T> &b,
                 std::vector<T> &coef)
{ return conv_t<T>::conv(a,b,coef); }


// unsigned __int64 get_cycle_count()
// {
// // #pragma warning ( push )
//  #pragma warning ( 2 : 4235 )
//  __asm _emit 0x0f;
//  __asm _emit 0x31;
// /*#pragma warning ( pop )*/
// }

// #include <boost/math/constants/constants.hpp>
//make a discrete array of sine or cosine wave
//A : peak amplitude
//f : frequency
//phy : original phase
//B : dc offset
//sr : sample rate
//x,y : output array
//samples : output array size,sample points
// int32_t sine(double A,double f,double phy,double B,double sr,double *x,double *y,int32_t samples)
// {
//     for (int32_t i = 0;i < samples;i ++) {
//         x[i] = 1.0 / sr * double(i);
//         y[i] = A * sin(2 * boost::math::constants::pi<double>() * f * x[i] + phy) + B;
//     }
//     return 0;
// }

// int32_t cosine(double A,double f,double phy,double B,double sr,double *x,double *y,int32_t samples)
// {
//     for (int32_t i = 0;i < samples;i ++) {
//         x[i] = 1.0 / sr * double(i);
//         y[i] = A * cos(2 * boost::math::constants::pi<double>() * f * x[i] + phy) + B;
//     }
//     return 0;
// }

#endif
