#include "logger_msg.h"
#include "list.hpp"
#include "integers.hpp"
#include "exception.hpp"
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

static logger_msg g_logger_msg;

msg_log_t::msg_log_t():
    result(0)
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

logger_msg &logger_msg::instance()
{
    return g_logger_msg;
}

int logger_msg::add(int32_t result,const char *fmt,...)
{
    if (!is_en(Message) && !is_en(File)) {
        return 0;
    }

    lock();

    msg_log_t *msg = new msg_log_t;

    char msg_buf[512] = "";
    va_list ap;
    va_start(ap,fmt);
    vsprintf(msg_buf,fmt,ap);
    va_end(ap);
    msg->msg = msg_buf;
    msg->result = result;

    if (is_en(File)) {
        fprintf(_fp,"%s %50s %#010x\n",
                msg->time.c_str(),
                msg->msg.c_str(),
                msg->result);
        fflush(_fp);
    }

    if (is_en(Message)) {
       logs()->push_back(msg);

#if defined GUI_MFC
        if (m_wnd) {
            vector<HWND>::iterator iter;
            for (iter = m_wnd->begin();iter != m_wnd->end();iter ++) {
                ::PostMessage(*iter,WM_MSG_LOG,0,0);
            }
        }
#else
        if (_callback) {
            _callback();
        }
#endif
    }

    unlock();

    if (result && _en_expt) {
        throw ::runtime_error(msg->msg);
    }

    return result;
}


