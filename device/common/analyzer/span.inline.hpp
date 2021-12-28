#include "span.hpp"
#include "algo_math.hpp"

span::span()
{

}

void span::set_star(double star)
{
    limit_after(_full_range_star,star);
    save();
    _star = star;
}

void span::set_stop(double stop)
{
    limit_before(_full_range_stop,stop);
    save();
    _stop = stop;
}

void span::set_center(double freq)
{
    limit_between(_full_range_star,_full_range_stop,freq);

    double span_cur = _stop - _star;

    save();

    _star = freq - span_cur / 2.0;
    limit_after(_full_range_star,_star);

    _stop = _star + span_cur;
    limit_before(_full_range_stop,_stop);
}

void span::set_span(double range)
{
    double freq_center = (_star + _stop) / 2.0;

    save();

    _star = freq_center - range;
    limit_after(_full_range_star,_star);

    _stop = freq_center + range;
    limit_before(_full_range_stop,_stop);
}

void span::set_span(span_t type)
{
    switch (type) {
    case span_t::Full : {
        save();
        _star = _full_range_star;
        _stop = _full_range_stop;
        return;
    }
    case span_t::Zero : { return save(); }
    case span_t::Last : { return restore(); }
    }
}

void span::save()
{
    _star_last = _star;
    _stop_last = _stop;
}

void span::restore()
{
    _star = _star_last;
    _stop = _stop_last;
}
