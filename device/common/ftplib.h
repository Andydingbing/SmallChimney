#ifndef FTPLIB_H
#define FTPLIB_H

#include <limits.h>
#include <inttypes.h>

// FtpAccess() type codes
#define FTPLIB_DIR 1
#define FTPLIB_DIR_VERBOSE 2
#define FTPLIB_FILE_READ 3
#define FTPLIB_FILE_WRITE 4

// FtpAccess() mode codes
#define FTPLIB_ASCII 'A'
#define FTPLIB_IMAGE 'I'
#define FTPLIB_TEXT FTPLIB_ASCII
#define FTPLIB_BINARY FTPLIB_IMAGE

// connection modes
#define FTPLIB_PASSIVE 1
#define FTPLIB_PORT 2

// connection option names
#define FTPLIB_CONNMODE 1
#define FTPLIB_CALLBACK 2
#define FTPLIB_IDLETIME 3
#define FTPLIB_CALLBACKARG 4
#define FTPLIB_CALLBACKBYTES 5

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__UINT64_MAX)
typedef uint64_t fsz_t;
#else
typedef uint32_t fsz_t;
#endif

typedef struct NetBuf netbuf;
typedef int (*FtpCallback)(netbuf *nControl, fsz_t xfered, void *arg);

typedef struct FtpCallbackOptions {
    FtpCallback cbFunc;        // function to call
    void *cbArg;               // argument to pass to function
    unsigned int bytesXferred; // callback if this number of bytes transferred
    unsigned int idleTime;     // callback if this many milliseconds have elapsed
} FtpCallbackOptions;


void FtpInit(void);
char *FtpLastResponse(netbuf *nControl);
int FtpConnect(const char *host, netbuf **nControl);
int FtpOptions(int opt, long val, netbuf *nControl);
int FtpSetCallback(const FtpCallbackOptions *opt, netbuf *nControl);
int FtpClearCallback(netbuf *nControl);
int FtpLogin(const char *user, const char *pass, netbuf *nControl);
int FtpAccess(const char *path, int typ, int mode, netbuf *nControl, netbuf **nData);
int FtpRead(void *buf, int max, netbuf *nData);
int FtpWrite(const void *buf, int len, netbuf *nData);
int FtpClose(netbuf *nData);
int FtpSite(const char *cmd, netbuf *nControl);
int FtpSysType(char *buf, int max, netbuf *nControl);
int FtpMkdir(const char *path, netbuf *nControl);
int FtpChdir(const char *path, netbuf *nControl);
int FtpCDUp(netbuf *nControl);
int FtpRmdir(const char *path, netbuf *nControl);
int FtpPwd(char *path, int max, netbuf *nControl);
int FtpNlst(const char *output, const char *path, netbuf *nControl);
int FtpDir(const char *output, const char *path, netbuf *nControl);
int FtpSize(const char *path, unsigned int *size, char mode, netbuf *nControl);
#if defined(__UINT64_MAX)
int FtpSizeLong(const char *path, fsz_t *size, char mode, netbuf *nControl);
#endif
int FtpModDate(const char *path, char *dt, int max, netbuf *nControl);
int FtpGet(const char *output, const char *path, char mode, netbuf *nControl);
int FtpPut(const char *input, const char *path, char mode, netbuf *nControl);
int FtpRename(const char *src, const char *dst, netbuf *nControl);
int FtpDelete(const char *fnm, netbuf *nControl);
void FtpQuit(netbuf *nControl);

#ifdef __cplusplus
};
#endif

#endif // FTPLIB_H
