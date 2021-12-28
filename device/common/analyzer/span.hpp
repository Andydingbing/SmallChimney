#ifndef COMMON_ANALYZER_SPAN_HPP
#define COMMON_ANALYZER_SPAN_HPP

#include "enum.h"

namespace ns_span {

BETTER_ENUM(span_t,int32_t,Full,Zero,Last)

} // namespace ns_span

class span {
public:
    typedef ns_span::span_t span_t;

    span();

    double star() const { return _star; }
    double stop() const { return _stop; }
    void set_star(double star);
    void set_stop(double stop);

    void set_full_range_star(const double star) { _full_range_star = star; }
    void set_full_range_stop(const double stop) { _full_range_stop = stop; }

    void set_center(double freq);
    void set_span(double range);
    void set_span(span_t type);

private:
    void save();
    void restore();

private:
    double _star;
    double _stop;

    double _star_last;
    double _stop_last;

    double _full_range_star;
    double _full_range_stop;
};

#endif // COMMON_ANALYZER_SPAN_HPP
