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

#ifndef RD_UTILITIES_ALGO_FIT_H
#define RD_UTILITIES_ALGO_FIT_H

#include "global_header.h"
#include "exception.hpp"
#include "algo_math.hpp"

// x(0)^n + x(1)^n + x(2)^n + ... + x(x.size() - 1)^n
template <typename T,typename data_type = float_traits<T>>
typename data_type::data_t sum_pow(const std::vector<typename data_type::data_t> &x,size_t n)
{
    typename data_type::data_t sum = data_type::_0();
    for (size_t i = 0;i < x.size();i ++) {
        sum += pow(x[i],n);
    }
    return sum;
}

// Value of polynomial
// coef(0) + coef(1)*x^1 + coef(2)*x^2 + ... + coef(coef.size() - 1)*x^n
template <typename T,typename data_type = float_traits<T>>
typename data_type::float_t polynomial(const std::vector<T> &coef,T x)
{
    typename data_type::float_t y = 0.0;
    for (size_t i = 0;i < coef.size();i ++) {
        y += data_type::value_float(coef[i] * pow(x,i));
    }
    return y;
}

template <typename T,typename data_type = float_traits<T>>
typename data_type::float_t polynomial(const T *coef,size_t order,T x)
{
    typename data_type::float_t y = 0.0;
    for (size_t i = 0;i < order + 1;i ++) {
        y += data_type::value_float(coef[i] * pow(x,i));
    }
    return y;
}

// Coefs to polynomial
template <typename T,typename data_type = float_traits<T>>
std::string polynomial(const std::vector<T> &coef)
{
    std::string out;

    if (coef.size() < 1) {
        return out;
    }

    boost::format tf_0("%.16f");
    boost::format tf_1("%+.16fx^%d");
    boost::format tf_2("%-.16fx^%d");

    out = "f(x) = ";
    out += (tf_0 % data_type::value_float(coef[0])).str();
    for (size_t i = 1;i < coef.size();i ++) {
        if (coef[i] > 0) {
            tf_1 % data_type::value_float(coef[i]);
            tf_1 % i;
            out += tf_1.str();
        } else if (coef[i] < 0) {
            tf_2 % data_type::value_float(coef[i]);
            tf_2 % i;
            out += tf_2.str();
        }
    }
    return out;
}

template <typename T,typename data_type = float_traits<T>>
std::string polynomial(const T *coef,size_t order)
{
    std::vector<T> _coef;

    for (size_t i = 0;i <= order;i ++) {
        _coef.push_back(coef[i]);
    }
    return polynomial<T>(_coef);
}

/*
 * Resolve polynomial
 *
 * 1.0*a + 2.0*b + 3.0*c = 1.0 // x : 1.0,2.0,3.0 // y : 1.0
 * 4.0*a + 5.0*b + 6.0*c = 2.0 // x : 4.0,5.0,6.0 // y : 2.0
 * 7.0*a + 8.0*b + 9.0*c = 3.0 // x : 7.0,8.0,9.0 // y : 3.0
 */
template <typename T,typename data_type = float_traits<T>>
void polynomial(std::vector<std::vector<typename data_type::data_t>> x,
                std::vector<typename data_type::data_t> y,
                std::vector<typename data_type::data_t> &coef)
{
    /*
     * dimen_total === 3;
     *
     * x0 * a + x1 * b + x2 * c = y0; --- S(1) // dimen_cur = 3; dimen_cur_total = 3;
     * x3 * a + x4 * b + x5 * c = y1; --- S(2) // dimen_cur = 2; dimen_cur_total = 3;
     * x6 * a + x7 * b + x8 * c = y2; --- S(3) // dimen_cur = 1; dimen_cur_total = 3;
     *              |
     *              v
     * a + x1/x0 * b + x2/x0 * c = y0/x0; --- S(1) // dimen_cur = 3; dimen_cur_total = 3;
     * a + x4/x3 * b + x5/x3 * c = y1/x3; --- S(2) // dimen_cur = 2; dimen_cur_total = 3;
     * a + x7/x6 * b + x8/x6 * c = y2/x6; --- S(3) // dimen_cur = 1; dimen_cur_total = 3;
     *              |
     *              v
     * S(1)
     * S(2) - S(1) // dimen_cur = 2; dimen_cur_total = 2;
     * S(3) - S(1) // dimen_cur = 1; dimen_cur_total = 2;
     *              |
     *              v
     *            ......
     */

    typedef typename data_type::data_t data_t;

    RD_ASSERT_THROW(x.size() == y.size());

    const size_t dimen_total = x.size();
    size_t dimen_cur_total = dimen_total;
    size_t dimen_cur = dimen_total;
    data_t div = data_type::_0();

    coef.clear();
    for (size_t i = 0;i < dimen_total;i ++) {
        coef.push_back(data_type::_0());
    }

    for (dimen_cur = dimen_total;dimen_cur > 0;dimen_cur --) {
        dimen_cur_total = dimen_total - dimen_cur;
        for (size_t i = dimen_cur_total;i < dimen_total;i ++) {
            div = x[i][dimen_cur_total];
            if (!data_type::equal_0(div) && !data_type::equal_1(div)) {
                for (size_t j = dimen_cur_total;j < dimen_total;j ++) {
                    x[i][j] /= div;
                }
                y[i] /= div;
            }
        }
        for (size_t i = dimen_cur_total + 1;i < dimen_total;i ++) {
            if (!data_type::equal_0(x[i][dimen_cur_total])) {
                for (size_t j = dimen_cur_total;j < dimen_total;j ++) {
                    x[i][j] -= x[dimen_cur_total][j];
                }
                y[i] -= y[dimen_cur_total];
            }
        }
    }

    coef[dimen_total - 1] = y[dimen_total - 1];
    for (size_t i = dimen_total - 2;i > 0;i --) {
        for (size_t j = i + 1;j < dimen_total;j ++) {
            y[i] -= x[i][j] * coef[j];
        }
        coef[i] = y[i];
    }
    for (size_t j = 1;j < dimen_total;j ++) {
        y[0] -= x[0][j] * coef[j];
    }
    coef[0] = y[0];
}

template <typename T,typename data_type = float_traits<T>>
void polynomial(std::vector<typename data_type::data_t> x,
                std::vector<typename data_type::data_t> y,
                std::vector<typename data_type::data_t> &coef)
{
    const size_t n_x = x.size();
    const size_t n_y = y.size();

    if (n_y * n_y != n_x) {
        throw index_error(RD_THROW_SITE_INFO("Not matched array size of x and y"));
    }

    typedef typename data_type::data_t data_t;

    std::vector<std::vector<data_t>> X;

    for (size_t i = 0;i < n_y;i ++) {
        X.push_back(std::vector<data_t>());
        for (size_t j = 0;j < n_y;j ++) {
            X[i].push_back(x[i * n_y + j]);
        }
    }
    polynomial<data_t>(X,y,coef);
}


/*
 * polyfit(Least Squares)
 *
 * e.g.
 * x : 1.0,2.0,3.0
 * y : 4.0,5.0,7.0
 *
 * So the points are (1.0,4.0),(2.0,5.0),(3.0,7.0).
 * If n = 2,the function will make an equation like "a + b*x + c*x^2 = 0",
 * and coef will store a,b,c.
 */
template <typename T,typename data_type = float_traits<T>>
void polyfit(const std::vector<typename data_type::data_t> &x,
             const std::vector<typename data_type::data_t> &y,
             size_t n,
             std::vector<typename data_type::data_t> &coef)
{
    typedef typename data_type::data_t data_t;

    RD_ASSERT_THROW(x.size() == y.size());

    std::vector<data_t> xx;
    std::vector<data_t> YY;
    std::vector<std::vector<data_t>> XX;

    coef.clear();
    coef.assign(n + 1,data_type::_0());
    for (size_t i = 0;i <= n;i ++) {
        xx.clear();
        for (size_t j = 0;j <= n;j ++) {
            xx.push_back(sum_pow<data_t>(x,i + j));
        }
        XX.push_back(xx);
    }
    for (size_t i = 0;i <= n;i ++) {
        YY.push_back(data_type::_0());
        for (size_t j = 0;j < x.size();j ++) {
            YY[i] += pow(x[j],i) * y[j];
        }
    }
    polynomial<data_t>(XX,YY,coef);
}

/*
 * Lagrange interpolation
 *
 * e.g.
 * x : 4.0,5.0,6.0
 * y : 10.0,5.25,1.0
 *
 * So :
 * f(4.0) = 10.0;
 * f(5.0) = 5.25;
 * f(6.0) = 1
 *
 * The function will make an equation such as f(x) = a + b*x + c*x^2,
 * and coef will store a,b,c
 */
template <typename T,typename data_type = float_traits<T>>
void lagrange(const std::vector<typename data_type::data_t> &x,
              const std::vector<typename data_type::data_t> &y,
              std::vector<typename data_type::data_t> &coef)
{
    typedef typename data_type::data_t data_t;

    RD_ASSERT_THROW(x.size() == y.size());

    const size_t n = x.size();
    data_t div = data_type::_1();
    size_t degree = 0;

    std::vector<data_t> b;
    std::vector<data_t> l;
    std::vector<std::vector<data_t>> ll;

    coef.assign(n,data_type::_1());

    b.push_back(data_type::_0());
    b.push_back(data_type::_1());
    l.assign(n,data_type::_0());
    ll.assign(n,l);

    for (size_t i = 0;i < n;i ++) {
        div = data_type::_1();
        degree = 0;
        coef.assign(n,data_type::_1());
        for (size_t j = 0;j < n;j ++) {
            if (i == j) {
                b[0] = data_type::_1();
                b[1] = data_type::_0();
                conv(coef,degree,b,0,ll[i]);
            } else {
                b[0] = -1 * x[j];
                b[1] = data_type::_1();
                div *= (x[i] - x[j]);
                conv(coef,degree,b,1,ll[i]);
                degree ++;
            }
            coef = ll[i];
        }
        for (size_t j = 0;j < n;j ++) {
            ll[i][j] /= div;
            ll[i][j] *= y[i];
        }
    }

    for (size_t i = 0;i < n;i ++) {
        coef[i] = 0;
        for (size_t j = 0;j < n;j ++) {
            coef[i] += ll[j][i];
        }
    }
}


/*
 * cubic spline interpolation
 * x,y : (x,y)
 * a,
 * b,
 * c,
 * d   : coefficients of S(x) = a + b*(x-x[i]) + c*(x-x[i])^2 + d*(x-x[i])^3
 * end : end condition
 * A,B : clamped's end condition
 */
namespace ns_spline {

enum end {
    natural,   // S"(0) = S"(n-1) = 0
    clamped,   // S'(0) = A,S'(n-1) = B
    not_a_knot // S'''(0) = S'''(1),S'''(n-2) = S'''(n-1)
};

} // namespace ns_spline

template <typename T,typename data_type = float_traits<T>>
void spline(const std::vector<typename data_type::data_t> &x,
            const std::vector<typename data_type::data_t> &y,
            std::vector<typename data_type::data_t> &a,
            std::vector<typename data_type::data_t> &b,
            std::vector<typename data_type::data_t> &c,
            std::vector<typename data_type::data_t> &d,
            ns_spline::end end = ns_spline::not_a_knot,
            typename data_type::data_t A = data_type::_0(),
            typename data_type::data_t B = data_type::_0())
{
    typedef typename data_type::data_t data_t;

    RD_ASSERT_THROW(x.size() == y.size());

    const size_t n = x.size();

    std::vector<data_t> X; X.assign(n * n,data_type::_0());
    std::vector<data_t> Y; Y.assign(n,    data_type::_0());
    std::vector<data_t> h; h.assign(n - 1,data_type::_0());
    std::vector<data_t> m; m.assign(n,    data_type::_0());

    for (size_t i = 0;i < n - 1;i ++) {
        h[i] = x[i + 1] - x[i];
    }

    if (end == ns_spline::natural) {
        X[0] = X[n * n - 1] = data_type::_1();
        Y[0] = Y[n - 1]     = data_type::_0();
    } else if (end == ns_spline::clamped) {
        X[0] = h[0] * 2;
        X[1] = h[0];
        X[n * n - 1] = h[n - 2] * 2;
        X[n * n - 2] = h[n - 3];
        Y[0] = (y[1] - y[0]) / h[0] - A;
        Y[0] *= 6;
        Y[n - 1] = B - (y[n - 1] - y[n - 2]) / h[n - 2];
        Y[n - 1] *= 6;
    } else if (end == ns_spline::not_a_knot) {
        X[0] = h[1] * -1;
        X[1] = h[0] + h[1];
        X[2] = h[0] * -1;
        X[n * n - 1] = h[n - 3] * -1;
        X[n * n - 2] = h[n - 3] + h[n - 2];
        X[n * n - 3] = h[n - 2] * -1;
        Y[0] = Y[n - 1] = 0;
    }

    for (size_t i = 1;i < n - 1;i ++) {
        X[i * n + i - 1] = h[i - 1];
        X[i * n + i]     = (h[i - 1] + h[i]) * 2;
        X[i * n + i + 1] = h[i];
    }

    for (size_t i = 1;i < n - 1;i ++) {
        Y[i] = (y[i + 1] - y[i]) / h[i] - (y[i] - y[i - 1]) / h[i - 1];
        Y[i] *= 6;
    }

    polynomial<data_t>(X,Y,m);

    a.assign(n - 1,data_type::_0());
    b.assign(n - 1,data_type::_0());
    c.assign(n - 1,data_type::_0());
    d.assign(n - 1,data_type::_0());
    for (size_t i = 0;i < n - 1;i ++) {
        a[i] = y[i];
        b[i] = (y[i + 1] - y[i]) / h[i] - h[i] * (2 * m[i] + m[i + 1]) / 6;
        c[i] = m[i] / 2;
        d[i] = (m[i + 1] - m[i]) / h[i] / 6;
    }
}

/*
 * 1 dimension data interpolation
 * x,y    : (x,y)
 * x1,y1  : output (x1,y1)
 * method : linear method
 * I      : interpolation factor
 */
namespace ns_interp {

enum method {
    linear,  // linear,default
    nearest, // nearest neighbor
    zero,    // zero
    spline   // cubic spline
};

} // namespace ns_interp

template<typename T,typename traits_t = float_traits<T>>
void interp1(const std::vector<T> &x,
             const std::vector<T> &y,
             std::vector<T> &x1,
             std::vector<T> &y1,
             ns_interp::method method = ns_interp::linear,
             int32_t I = 2)
{
    const size_t x_size = x.size();
    const size_t y_size = y.size();

    RD_ASSERT_THROW(x_size == y_size);

    x1.assign(x_size + (x_size - 1) * I,T(0));
    y1.assign(y_size + (y_size - 1) * I,T(0));

    size_t i = 0;

    for (i = 0;i < x_size - 1;++i) {
        x1[i * I] = x[i];
        y1[i * I] = y[i];

        for (int32_t j = 1;j < I;j ++) {
            x1[i * I + j] = (x[i + 1] - x[i]) / I * j + x[i];
        }
    }

    x1[i * I] = x[i];
    y1[i * I] = y[i];

    if (method == ns_interp::linear) {
        for (i = 0;i < x_size - 1;++i) {
            for (int32_t j = 1;j < I;++j) {
                y1[i * I + j] = (y[i + 1] - y[i]) / I * j + y[i];
            }
        }
    } else if (method == ns_interp::nearest) {
        for (i = 0;i < x_size - 1;++i) {
            for (int32_t j = 1;j < I;++j) {
                y1[i * I + j] = y[i + 1];
            }
        }
    } else if (method == ns_interp::zero) {
        for (i = 0;i < x_size - 1;++i) {
            for (int32_t j = 1;j < I;++j) {
                y1[i * I + j] = 0;
            }
        }
    } else if (method == ns_interp::spline) {
        std::vector<T> a;
        std::vector<T> b;
        std::vector<T> c;
        std::vector<T> d;
        spline(x,y,a,b,c,d,ns_spline::not_a_knot);

        double h = 0.0;

        for (i = 0;i < x_size - 1;++i) {
            for (int32_t j = 1;j < I;++j) {
                h = x1[i * I + j] - x[i];
                y1[i * I + j] = a[i] + b[i] * h + c[i] * h * h + d[i] * h * h * h;
            }
        }
    }
}

#endif // RD_UTILITIES_ALGO_FIT_H
