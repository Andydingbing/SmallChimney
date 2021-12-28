#include "logger_com.h"
#include "list.hpp"
#include <iostream>
#include <cstdarg> // bring in va
#include <fmt/core.h>

#ifdef PLATFORM_WIN32
    #include <Windows.h>
	#include <WinDef.h>
	#include <WinBase.h>
    #include <wingdi.h>
    #include <WinCon.h>
#elif defined PLATFORM_LINUX
    #include <sys/time.h>
#endif

com_log_t::com_log_t() :
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

int32_t logger_com::add(int32_t result,const std::string &write,const std::string &read)
{
    if (!is_en(Message) && !is_en(File)) {
        return 0;
    }

    lock();

    com_log_t *com = new com_log_t;

    com->write = write;
    com->read = read;
    com->result = result;

    if (is_en(File)) {
        fprintf(_fp,"%s %50s %50s %#010x\n",
                com->time.c_str(),
                com->write.c_str(),
                com->read.c_str(),
                com->result);
        fflush(_fp);
    }

    if (is_en(Message)) {
       _logs.push_back(com);

#if defined GUI_MFC
        if (_wnd) {
            vector<HWND>::iterator iter;
            for (iter = _wnd->begin();iter != _wnd->end();++iter) {
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
    return result;
}


