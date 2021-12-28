#ifndef LOG_H
#define LOG_H

#include "global_header.h"
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

#if defined(PLATFORM_WIN32) && defined (GUI_MFC)
    #include <WinUser.h>

    #define WM_MSG_LOG (WM_USER + 1000)
    #define WM_REG_LOG (WM_USER + 1001)
#endif

#define Log log_t::instance()

class API log_t : noncopyable
{
public:
    struct API msg_log_t {
        int32_t result;
        std::string msg;
        std::string time;

        msg_log_t();
    };

    struct API reg_log_t {
        int32_t  result;
        uint32_t addr;
        uint32_t w;
        uint32_t r;
        std::string fpga;
        std::string time;

        reg_log_t();
    };

public:
    typedef boost::posix_time::ptime ptime;

    enum log_sw_t {
        RD_LOG_ALL_OFF   = 0x00000000,
        RD_LOG_PROMPT    = 0x00000001,
        RD_LOG_MESSAGE   = 0x00000004,
        RD_LOG_MESSAGE_F = 0x00000008,
        RD_LOG_REG       = 0x00000010,
        RD_LOG_REG_F     = 0x00000020,
        RD_LOG_TRACE     = 0x00000040,
        RD_LOG_ALL_ON    = 0x7FFFFFFF
    };

public:
    log_t();
    virtual ~log_t();
    static log_t &instance();

#ifdef GUI_MFC
    void init(HWND);
    void init(std::vector<HWND>);
private:
    std::vector<HWND> *m_wnd;
#else
    void init(void (*msg_callback)(),void (*reg_callback)());
private:
    void (*m_msg_callback)();
    void (*m_reg_callback)();
#endif

public:
    size_t msgs();
    size_t regs();

    msg_log_t *msg(const size_t idx);
    reg_log_t *reg(const size_t idx);

    bool is_en(log_sw_t);
    void en(log_sw_t,bool);
    void set_default();
    void set_last_err(const char *format,...);
    void set_last_err(int, const char *format,...);
    char *last_err() { return m_last_err; }

    virtual void stdprintf(const char *fmt,...);

    virtual int add_msg(const char *fmt,...);
    virtual int add_msg(int32_t result,const char *fmt,...);
    virtual int add_reg(int32_t result,const std::string &fpga,uint32_t addr,uint32_t w = 0xFFFFFFFF,uint32_t r = 0xFFFFFFFF);

public:
    tm time_current();
    boost::posix_time::time_duration time_elapsed();

private:
    void set_log_time(msg_log_t *lg);
    void set_log_time(reg_log_t *lg);

private:
    ptime _tstar;
    ptime _tcurr;
    FILE *m_fp_msg;
    FILE *m_fp_reg;
    char m_last_err[512];
    uint64_t m_sw;
#ifdef PLATFORM_WIN32
    void *m_console;
#endif
};

#endif
