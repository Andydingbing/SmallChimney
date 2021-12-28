#ifndef LOGGER_H
#define LOGGER_H

#include "list.hpp"
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

#if defined(PLATFORM_WIN32) && defined (GUI_MFC)
    #include <WinUser.h>

    #define WM_MSG_LOG (WM_USER + 1000)
    #define WM_REG_LOG (WM_USER + 1001)
#endif

class API logger_base : noncopyable
{
public:
    typedef boost::posix_time::ptime ptime;

    enum sw_t {
        All_Off = 0x00000000,
        Message = 0x00000001,
        Prompt  = 0x00000002,
        File    = 0x00000004,
        All_On  = 0x7FFFFFFF
    };

public:
    logger_base();
    logger_base(const std::string &path);
    virtual ~logger_base();

#ifdef GUI_MFC
    void init(HWND);
    void init(std::vector<HWND>);
private:
    std::vector<HWND> *_wnd;
#else
    void init(void (*callback)());
    void set_callback(void (*callback)());
protected:
    void (*_callback)();
#endif

public:
    void lock() { _lock.lock(); }
    void unlock() { _lock.unlock(); }

    bool is_en(const sw_t) const;
    void en(const sw_t,const bool);

    void set_en_expt(const bool en);
    void set_path(const std::string &path);
    void set_default();
    void set_last_err(const char *format,...);
    void set_last_err(int, const char *format,...);
    std::string &last_err() { return _last_err; }

    virtual void stdprintf(const char *fmt,...);

    void set_time_curr();
    ptime &time_curr() { return _tstar; }
    boost::posix_time::time_duration time_elapsed();

private:
    void constructer();

protected:
    ptime _tstar;
    ptime _tcurr;

    boost::mutex _lock;

    bool _en_expt;
    uint64_t _sw;
    std::string _path;
    FILE *_fp;
    std::string _last_err;

#ifdef PLATFORM_WIN32
    void *_console;
#endif

};

#endif
