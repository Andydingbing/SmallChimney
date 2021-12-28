#include "logger_reg.h"
#include "list.hpp"
#include <iostream>
#include <cstdarg> // bring in va
#include <fmt/core.h>
#include <boost/thread.hpp>

#ifdef PLATFORM_WIN32
    #include <Windows.h>
	#include <WinDef.h>
	#include <WinBase.h>
    #include <wingdi.h>
    #include <WinCon.h>
#elif defined PLATFORM_LINUX
    #include <sys/time.h>
#endif

static logger_reg g_logger_reg;

reg_log_t::reg_log_t() :
    result(0),
    addr(0xffff),
    w(0xFFFFFFFF),
    r(0xFFFFFFFF)
{
    std::tm t = to_tm(boost::posix_time::microsec_clock::local_time());

    time = fmt::format("{:4d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}",
                       (t.tm_year + 1900),
                       (t.tm_mon + 1),
                       t.tm_mday,
                       t.tm_hour,
                       t.tm_min,
                       t.tm_sec);
}

logger_reg &logger_reg::instance()
{
    return g_logger_reg;
}

int logger_reg::add(int32_t result, const std::string &chip, uint32_t addr, uint32_t w, uint32_t r)
{
    if (!is_en(Message) && !is_en(File)) {
        return 0;
    }

    lock();

    reg_log_t *reg = new reg_log_t;

    reg->result = result;
    reg->addr = addr;
    reg->w = w;
    reg->r = r;
    reg->chip = chip;

    if (w != 0xffffffff) {
        if (is_en(File)) {
            fprintf(_fp,"%s	%4s  %#06x  %#010x  %16s  %d\n",
                    reg->time.c_str(),
                    reg->chip.c_str(),
                    reg->addr,
                    reg->w,
                    "",
                    reg->result);
            fflush(_fp);
        }
    }

    if (r != 0xffffffff) {
        if (is_en(File)) {
            fprintf(_fp,"%s	%4s  %#06x  %16s  %#010x  %d\n",
                    reg->time.c_str(),
                    reg->chip.c_str(),
                    reg->addr,
                    "",
                    reg->r,
                    reg->result);
            fflush(_fp);
        }
    }

    if (is_en(Message)) {
        logs()->push_back(reg);

#if defined GUI_MFC
        if (m_wnd) {
            vector<HWND>::iterator iter;
            for (iter = m_wnd->begin();iter != m_wnd->end();iter ++) {
                ::PostMessage(*iter,WM_REG_LOG,0,0);
            }
        }
#else
        if (_callback) {
            _callback();
        }
#endif
    }

    unlock();
    return result;
}


