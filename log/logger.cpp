#include "logger.h"
#include "list.hpp"
#include <iostream>
#include <cstdarg> // bring in va
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

using namespace std;
using namespace boost;

logger_base::logger_base()
{
#ifdef PLATFORM_WIN32
    _path = "C://log//log.log";
#elif defined PLATFORM_LINUX
    _path = "/var/log/log.log";
#endif

    constructer();
}

logger_base::logger_base(const string &path)
{
    _path = path;
    constructer();
}

void logger_base::constructer()
{
    _tstar = boost::posix_time::microsec_clock::local_time();
    _tcurr = _tstar;

    _en_expt = false;

#ifdef PLATFORM_WIN32
    _console = nullptr;
#endif

#ifdef GUI_MFC
    _wnd = nullptr;
#else
    _callback = nullptr;
#endif

    _fp = nullptr;

    if (!_path.empty()) {
        _fp = fopen(_path.c_str(),"a");
    }

    set_default();
}

logger_base::~logger_base()
{
    if (_fp != nullptr) {
        fclose(_fp);
        _fp = nullptr;
    }

#ifdef GUI_MFC
    SAFE_DELETE_VECTOR(_wnd);
#endif

#ifdef PLATFORM_WIN32
    if (_console) { FreeConsole(); }
#endif
}

#ifdef GUI_MFC
void logger_base::init(HWND wnd)
{
    SAFE_DELETE_VECTOR(_wnd);
    _wnd = new vector<HWND>;
    _wnd->push_back(wnd);
}

void logger_base::init(vector<HWND> wnd)
{
    vector<HWND>::iterator iter;

    SAFE_DELETE_VECTOR(_wnd)
    _wnd = new vector<HWND>;

    for (iter = wnd.begin();iter != wnd.end();iter ++) {
        _wnd->push_back(*iter);
    }
}
#else
void logger_base::init(void (*callback)())
{
    _callback = callback;
}

void logger_base::set_callback(void (*callback)())
{
    _callback = callback;
}
#endif

bool logger_base::is_en(const sw_t sw) const
{
    if (sw == All_On) {
        return _sw == All_On ? true : false;
    }
    if (sw == All_Off) {
        return _sw == All_Off ? true : false;
    }
    return (_sw & sw) ? true : false;
}

void logger_base::en(const sw_t sw,const bool en)
{
    if (en) {
        _sw |= uint64_t(sw);
    } else {
        _sw &= (~uint64_t(sw));
    }
}

void logger_base::set_en_expt(const bool en)
{
    _en_expt = en;
}

void logger_base::set_path(const string &path)
{
    if (path.empty()) {
        return;
    }

    if (_fp != nullptr) {
        fclose(_fp);
        _fp = nullptr;
    }

    _path = path;
    _fp = fopen(_path.c_str(),"a");
}

void logger_base::set_default()
{
#ifdef _DEBUG
    _sw = Message | Prompt;
#else
    _sw = Message;
#endif
}

void logger_base::set_time_curr()
{
    _tcurr = boost::posix_time::microsec_clock::local_time();
}

boost::posix_time::time_duration logger_base::time_elapsed()
{
    set_time_curr();
    return _tcurr - _tstar;
}

void logger_base::stdprintf(const char *fmt,...)
{	
    if (!is_en(Prompt)) {
        return;
    }

    string buf = to_simple_string(time_elapsed());
    buf += " ";
    char fmt_buf[512] = "";
    buf.copy(fmt_buf,buf.length(),0);

	va_list ap;
    va_start(ap,fmt);
    vsprintf(&fmt_buf[strlen(fmt_buf)],fmt,ap);
	va_end(ap);
    buf = fmt_buf;

#ifdef PLATFORM_WIN32
    if (!_console) {
		AllocConsole();
        _console = GetStdHandle(STD_OUTPUT_HANDLE);
	}
    WriteConsoleA(_console,buf.c_str(),DWORD(buf.length()),nullptr,nullptr);
#elif defined RD_PLATFORM_LINUX
    printf("%s",buf.c_str());
#endif
}

void logger_base::set_last_err(const char *format,...)
{
    _last_err.clear();
    _last_err.reserve(512);

    va_list ap;
    va_start(ap,format);
    vsprintf(const_cast<char *>(_last_err.c_str()),format,ap);
    va_end(ap);
}

void logger_base::set_last_err(int result,const char *format,...)
{
    _last_err.clear();
    _last_err.reserve(512);

    va_list ap;
    va_start(ap,format);
    vsprintf(const_cast<char *>(_last_err.c_str()),format,ap);
    va_end(ap);
}

