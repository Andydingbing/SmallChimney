#ifndef FTP_H
#define FTP_H

#include <string>
#include "ftplib.h"

namespace rd {

#define FTP_CHECK(func) \
    if ( (ret = func) != 1 ) { \
        return false; \
    }

#define DECL_FTP_PUT(report) \
    public: int32_t ftp_put##report();

#define IMPL_FTP_PUT(class,report,remote,usr,pwd,database) \
int32_t class::ftp_put##report() \
{   ftp ftp_worker(remote,usr,pwd); \
    path p_host(report->full_path()); \
    path p_remote(database); \
    string file_name_remote(report->get_header().sn); \
    file_name_remote += "_"; \
    file_name_remote += p_host.filename().string(); \
    for (uint32_t i = 0;i < 3;i ++) { \
        if (ftp_worker.put(p_host.string(),p_remote.string(),file_name_remote)) { \
            return 0; \
        } else { \
            if (frontend::_ftp_retry_call_back.empty()) { \
                return -1; \
            } \
            if (frontend::_ftp_retry_call_back()) { \
                continue; \
            } else { \
                return -1; \
            } \
        } \
    } \
    return -1; \
}

class ftp
{
public:
    ftp(const std::string &host,const std::string &usr,const std::string &pwd) :
        _host(host), _usr(usr), _pwd(pwd) {}
    bool is_remote_available()
    {
        int ret = 0;
        netbuf *ctrl = nullptr;

        FtpInit();
        FTP_CHECK(FtpConnect(_host.c_str(),&ctrl));
        FTP_CHECK(FtpClose(ctrl));
        return true;
    }

    bool put(const std::string &local,const std::string &remote,const std::string &name)
    {
        int ret = 0;
        netbuf *ctrl = nullptr;

        FtpInit();
        FTP_CHECK(FtpConnect(_host.c_str(),&ctrl));
        FTP_CHECK(FtpOptions(FTPLIB_CONNMODE,FTPLIB_PASSIVE,ctrl));
        FTP_CHECK(FtpLogin(_usr.c_str(),_pwd.c_str(),ctrl));
        FTP_CHECK(FtpChdir(remote.c_str(),ctrl));
        FTP_CHECK(FtpPut(local.c_str(),name.c_str(),FTPLIB_IMAGE,ctrl));
        FTP_CHECK(FtpClose(ctrl));
        return true;
    }

    bool get(const std::string &local,const std::string &remote,const std::string &name)
    {
        int ret = 0;
        netbuf *ctrl = nullptr;

        FtpInit();
        FTP_CHECK(FtpConnect(_host.c_str(),&ctrl));
        FTP_CHECK(FtpClose(ctrl));
        return true;
    }

private:
    std::string _host;
    std::string _usr;
    std::string _pwd;
};

} // namespace rd

#endif // FTP_H
