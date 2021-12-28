#include "log.h"
#include "list.hpp"
#include <iostream>
#include <cstdarg> // bring in va
#include <fmt/core.h>
#include <boost/thread.hpp>

#ifdef PLATFORM_WIN32
    #define MSG_LOG_PATH "C://msg.log"
    #define REG_LOG_PATH "C://reg.log"

    #include <Windows.h>
	#include <WinDef.h>
	#include <WinBase.h>
    #include <wingdi.h>
    #include <WinCon.h>
#elif defined PLATFORM_LINUX
    #define MSG_LOG_PATH "/var/log/spc5/msg.log"
    #define REG_LOG_PATH "/var/log/spc5/reg.log"
    #include <sys/time.h>
#endif

static log_t g_log;
static ctd::dlist<log_t::msg_log_t *> g_msg_log = ctd::dlist<log_t::msg_log_t *>();
static ctd::dlist<log_t::reg_log_t *> g_reg_log = ctd::dlist<log_t::reg_log_t *>();

static boost::mutex g_log_lock;

log_t::msg_log_t::msg_log_t() : result(0),msg("\0"),time("\0") {}
log_t::reg_log_t::reg_log_t() : result(0),addr(0xffff),w(0xFFFFFFFF),r(0xFFFFFFFF),fpga("\0"),time("\0") {}

log_t::log_t()
{ 
    _tstar = boost::posix_time::microsec_clock::local_time();
    _tcurr = _tstar;

#ifdef PLATFORM_WIN32
    m_console = nullptr;
#endif

#ifdef GUI_MFC
    m_wnd = NULL;
#else
    m_msg_callback = nullptr;
    m_reg_callback = nullptr;
#endif

    m_fp_msg = fopen(MSG_LOG_PATH,"a");
    m_fp_reg = fopen(REG_LOG_PATH,"a");

    set_default();
}

log_t::~log_t()
{
    if (m_fp_msg != nullptr) {
        fclose(m_fp_msg);
    }

    if (m_fp_reg != nullptr) {
        fclose(m_fp_reg);
    }

#ifdef GUI_MFC
    SAFE_DELETE_VECTOR(m_wnd);
#endif

#ifdef PLATFORM_WIN32
    if (m_console) { FreeConsole(); }
#endif
}

log_t &log_t::instance()
{
    return g_log;
}

#ifdef GUI_MFC
void log_t::init(HWND wnd)
{
    SAFE_DELETE_VECTOR(m_wnd);
    m_wnd = new std::vector<HWND>;
    m_wnd->push_back(wnd);
}

void log_t::init(std::vector<HWND> wnd)
{
    std::vector<HWND>::iterator iter;

    SAFE_DELETE_VECTOR(m_wnd)
    m_wnd = new std::vector<HWND>;
    for (iter = wnd.begin();iter != wnd.end();iter ++) {
        m_wnd->push_back(*iter);
    }
}
#else
void log_t::init(void (*msg_callback)(),void (*reg_callback)())
{
    m_msg_callback = msg_callback;
    m_reg_callback = reg_callback;
}
#endif

size_t log_t::msgs()
{
    return g_msg_log.size();
}

size_t log_t::regs()
{
    return g_reg_log.size();
}

log_t::msg_log_t *log_t::msg(const size_t idx)
{
    return g_msg_log.at(idx);
}

log_t::reg_log_t *log_t::reg(const size_t idx)
{
    return g_reg_log.at(idx);
}

bool log_t::is_en(log_t::log_sw_t sw)
{
    if (RD_LOG_ALL_ON == sw) {
        return RD_LOG_ALL_ON == m_sw ? true : false;
    }
    if (RD_LOG_ALL_OFF == sw) {
        return RD_LOG_ALL_OFF == m_sw ? true : false;
    }
    return (m_sw & sw) ? true : false;
}

void log_t::en(log_t::log_sw_t sw,bool en)
{
    if (en) {
        m_sw |= uint64_t(sw);
    } else {
        m_sw &= (~uint64_t(sw));
    }
}

void log_t::set_default()
{
#ifdef _DEBUG
    m_sw = RD_LOG_MESSAGE | RD_LOG_PROMPT | RD_LOG_TRACE;
#else
    m_sw = RD_LOG_MESSAGE;
#endif
}

tm log_t::time_current()
{
    time_t t;
    time(&t);
    return *localtime(&t);
}

boost::posix_time::time_duration log_t::time_elapsed()
{
    _tcurr = boost::posix_time::microsec_clock::local_time();
    return _tcurr - _tstar;
}

void log_t::stdprintf(const char *fmt,...)
{	
    if (!(m_sw & RD_LOG_PROMPT)) {
		return;
    }

    std::string buf = to_simple_string(time_elapsed());
    buf += " ";
    char fmt_buf[512] = "";
    buf.copy(fmt_buf,buf.length(),0);

	va_list ap;
    va_start(ap,fmt);
    vsprintf(&fmt_buf[strlen(fmt_buf)],fmt,ap);
	va_end(ap);
    buf = fmt_buf;

#ifdef PLATFORM_WIN32
    if (!m_console) {
		AllocConsole();
        m_console = GetStdHandle(STD_OUTPUT_HANDLE);
	}
    WriteConsoleA(m_console,buf.c_str(),DWORD(buf.length()),nullptr,nullptr);
#elif defined RD_PLATFORM_LINUX
    printf("%s",buf.c_str());
#endif
}

void log_t::set_log_time(log_t::msg_log_t *lg)
{
    _tcurr = boost::posix_time::microsec_clock::local_time();
    std::tm tm_time = to_tm(_tcurr);

    lg->time = fmt::format("{:4d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}",
                           (tm_time.tm_year + 1900),
                           (tm_time.tm_mon + 1),
                           tm_time.tm_mday,
                           tm_time.tm_hour,
                           tm_time.tm_min,
                           tm_time.tm_sec);
}

void log_t::set_log_time(log_t::reg_log_t *lg)
{
    _tcurr = boost::posix_time::microsec_clock::local_time();
    std::tm tm_time = to_tm(_tcurr);

    lg->time = fmt::format("{:02d}:{:02d}:{:02d}",
                           tm_time.tm_hour,
                           tm_time.tm_min,
                           tm_time.tm_sec);
}

int log_t::add_msg(const char *fmt,...)
{
    if ((!(m_sw & RD_LOG_MESSAGE)) && (!(m_sw & RD_LOG_MESSAGE_F))) {
		return 0;
    }

    g_log_lock.lock();

    msg_log_t *msg = new msg_log_t;

    set_log_time(msg);

    char msg_buf[512] = "";
    va_list ap;
    va_start(ap,fmt);
    vsprintf(msg_buf,fmt,ap);
    va_end(ap);
    msg->msg = msg_buf;

    if (m_sw & RD_LOG_MESSAGE_F) {
        fprintf(m_fp_msg,"%s %50s %#010x\n",
                msg->time.c_str(),
                msg->msg.c_str(),
                msg->result);
        fflush(m_fp_msg);
	}

    if (m_sw & RD_LOG_MESSAGE) {
        g_msg_log.push_back(msg);

#if defined GUI_MFC
        if (m_wnd) {
            vector<HWND>::iterator iter;
            for (iter = m_wnd->begin();iter != m_wnd->end();iter ++)
                ::PostMessage(*iter,WM_MSG_LOG,0,0);
        }
#else
        if (m_msg_callback) {
            m_msg_callback();
        }
#endif
	}
    g_log_lock.unlock();
	return 0;
}

int log_t::add_msg(int32_t result,const char *fmt,...)
{
    if ((!(m_sw & RD_LOG_MESSAGE)) && (!(m_sw & RD_LOG_MESSAGE_F))) {
		return 0;
    }

    g_log_lock.lock();

    msg_log_t *msg = new msg_log_t;

    set_log_time(msg);

    char msg_buf[512] = "";
    va_list ap;
    va_start(ap,fmt);
    vsprintf(msg_buf,fmt,ap);
    va_end(ap);
    msg->msg = msg_buf;
    msg->result = result;

    if (m_sw & RD_LOG_MESSAGE_F) {
        fprintf(m_fp_msg,"%s %50s %#010x\n",
                msg->time.c_str(),
                msg->msg.c_str(),
                msg->result);
        fflush(m_fp_msg);
	}

    if (m_sw & RD_LOG_MESSAGE) {
       g_msg_log.push_back(msg);

#if defined GUI_MFC
        if (m_wnd) {
            vector<HWND>::iterator iter;
            for (iter = m_wnd->begin();iter != m_wnd->end();iter ++)
                ::PostMessage(*iter,WM_MSG_LOG,0,0);
        }
#else
        if (m_msg_callback) {
            m_msg_callback();
        }
#endif
	}
    g_log_lock.unlock();
    return result;
}

int log_t::add_reg(int32_t result, const std::string &fpga, uint32_t addr, uint32_t w, uint32_t r)
{
    if ((!(m_sw & RD_LOG_REG)) && (!(m_sw & RD_LOG_REG_F))) {
		return 0;
    }

    g_log_lock.lock();

    reg_log_t *reg = new reg_log_t;

    set_log_time(reg);
    reg->result = result;
    reg->addr = addr;
    reg->w = w;
    reg->r = r;
    reg->fpga = fpga;

    if (w != 0xffffffff) {
        if (m_sw & RD_LOG_REG_F) {
            fprintf(m_fp_reg,"%s	%4s  %#06x  %#010x  %16s  %d\n",
                    reg->time.c_str(),
                    reg->fpga.c_str(),
                    reg->addr,
                    reg->w,
                    "",
                    reg->result);
            fflush(m_fp_reg);
		}
	}
    if (r != 0xffffffff) {
        if (m_sw & RD_LOG_REG_F) {
            fprintf(m_fp_reg,"%s	%4s  %#06x  %16s  %#010x  %d\n",
                    reg->time.c_str(),
                    reg->fpga.c_str(),
                    reg->addr,
                    "",
                    reg->r,
                    reg->result);
            fflush(m_fp_reg);
		}
	}

    if (m_sw & RD_LOG_REG) {
        g_reg_log.push_back(reg);

#if defined GUI_MFC
        if (m_wnd) {
			vector<HWND>::iterator iter;
            for (iter = m_wnd->begin();iter != m_wnd->end();iter ++)
                ::PostMessage(*iter,WM_REG_LOG,0,0);
		}
#else
        if (m_reg_callback) {
            m_reg_callback();
        }
#endif
	}
    g_log_lock.unlock();
    return result;
}

void log_t::set_last_err(const char *format,...)
{
    memset(m_last_err,0,sizeof(m_last_err));

    va_list ap;
    va_start(ap,format);
    vsprintf(m_last_err,format,ap);
	va_end(ap);
    add_msg(-1,m_last_err);
}

void log_t::set_last_err(int result,const char *format,...)
{
    memset(m_last_err,0,sizeof(m_last_err));

    va_list ap;
    va_start(ap,format);
    vsprintf(m_last_err,format,ap);
	va_end(ap);
    add_msg(result,m_last_err);
}
