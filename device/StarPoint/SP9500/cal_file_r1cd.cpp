#include "cal_file_r1cd.h"
#include "cal_file_r1ab.h"
#include <string.h>
#include <errno.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#ifdef PLATFORM_WIN32
    #include <Windows.h>
    #include <WinBase.h>
    #include <windef.h>
    #include <io.h>
    #include <direct.h>
#elif defined PLATFORM_LINUX
    #include <sys/stat.h>
    #include <unistd.h>
#endif

// file size before adding r1c/d data,make it easy to arrive to r1c/d data head directly
#define R1A_FILE_SIZE 5541296

using namespace std;
using namespace boost;
using namespace ns_sp1401;
using namespace ns_sp2401;


int32_t fir_process(string path)
{
#ifdef PLATFORM_WIN32
    PROCESS_INFORMATION pinfo;
    STARTUPINFOA sinfo;

    ZeroMemory(&sinfo,sizeof(sinfo) );
    sinfo.cb = sizeof(sinfo);
    ZeroMemory(&pinfo,sizeof(pinfo) );

    if(!CreateProcessA(nullptr,LPSTR(path.c_str()),
                       nullptr,nullptr,FALSE,
                       NORMAL_PRIORITY_CLASS,
                       nullptr,nullptr,&sinfo,&pinfo)) {
        Log.set_last_err("cannot execute \"%s\"(%d)",path,GetLastError());
        return -1;
    }

    DWORD ret = WaitForSingleObject(pinfo.hProcess,5 * 60 * 1000);
    if (WAIT_TIMEOUT == ret) {
        Log.set_last_err("process %s timeout",path);
        return -1;
    }

    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
#else
#endif
    return 0;
}

int32_t cal_file_r1cd::gen_tx_filter_coef()
{
    string path = boost::filesystem::initial_path<boost::filesystem::path>().string()
                  + "/" + "tx_filter.exe";

    if (fir_process(path)) {
        return -1;
    }
    return 0;
}

int32_t cal_file_r1cd::gen_rx_freq_res()
{
    uint64_t freqRF = 0;
    uint64_t freqStarRF = 0;
    uint64_t freqStopRF = 0;
    uint64_t freqStepRF = 0;
    uint64_t freqCalled = 0;
    uint64_t freqStarRF_InBw = 0;
    uint64_t freqStopRF_InBw = 0;
    uint64_t freqStepRF_InBw = 0;

    double freqIF = 0.0;
    double freqStarIF = 0.0;
    double freqStopIF = 0.0;
    double freqStepIF = 0.0;

    uint32_t idx = 0;

    //80M
    FILE *fp_fr = fopen("C:\\CSECal\\rx_filter_80.txt","w");
    if (fp_fr == nullptr) {
        Log.set_last_err("Could not open file");
        return -1;
    }

    // (2G-43M)~~~(2G+43M),step 2M.
    rx_rf_fr_table::data_f_t tempDataRFFr_80[ul_filter_80M_freqs];
    // -43M~~~43M,step 2M.
    rx_if_fr_table::data_f_t tempDataIFFr_80[ul_filter_80M_freqs];

    set_bw(_80M);
    map2buf(cal_file::RX_RF_FR);
    freqStarRF = uint64_t(2e9 + ul_filter_80M_freq_star);
    freqStopRF = uint64_t(2e9 + ul_filter_80M_freq_stop);
    freqStepRF = uint64_t(ul_filter_freq_step);
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        m_rx_rf_fr->get(freqRF,&tempDataRFFr_80[idx]);
        idx ++;
    }

    idx = 0;
    map2buf(cal_file::RX_IF_FR);
    freqStarIF = ul_filter_80M_freq_star;
    freqStopIF = ul_filter_80M_freq_stop;
    freqStepIF = ul_filter_freq_step;
    for (freqIF = freqStarIF;freqIF <= freqStopIF;freqIF += freqStepIF) {
        m_rx_if_fr->get(freqIF,&tempDataIFFr_80[idx]);
        idx ++;
    }

    // Real IF frequency response,-43M~~~43M.
    for (idx = 0;idx < ul_filter_80M_freqs;idx ++) {
        tempDataIFFr_80[idx].pwr -= tempDataRFFr_80[idx].pwr;
    }

    map2buf(cal_file::RX_RF_FR);
    freqStarRF = rx_freq_star;
    freqStopRF = rx_freq_stop;
    freqStepRF = r1c::rx_freq_step_called;
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        freqCalled = freqRF < 100000000 ? 100000000 : freqRF;
        idx = 0;
        freqStarRF_InBw = uint64_t(freqCalled + ul_filter_80M_freq_star);
        freqStopRF_InBw = uint64_t(freqCalled + ul_filter_80M_freq_stop);
        freqStepRF_InBw = uint64_t(ul_filter_freq_step);
        for (uint64_t freq = freqStarRF_InBw;freq <= freqStopRF_InBw;freq += freqStepRF_InBw) {
            m_rx_rf_fr->get(freq,&tempDataRFFr_80[idx]);
            fprintf(fp_fr,"%.6f ",tempDataRFFr_80[idx].pwr + tempDataIFFr_80[idx].pwr);
            idx ++;
        }
        fprintf(fp_fr,"\n");
    }
    fclose(fp_fr);
    fp_fr = nullptr;

    //160M
    fp_fr = fopen("C:\\CSECal\\rx_filter_160.txt","w");
    if (fp_fr == nullptr) {
        Log.set_last_err("Could not open file");
        return -1;
    }

    // (2G-83M)~~~(2G+83M),step 2M.
    rx_rf_fr_table::data_f_t tempDataRFFr_160[ul_filter_160M_freqs];
    // -83M~~~83M,step 2M.
    rx_if_fr_table::data_f_t tempDataIFFr_160[ul_filter_160M_freqs];

    idx = 0;
    set_bw(_160M);
    map2buf(cal_file::RX_RF_FR);
    freqStarRF = uint64_t(2e9 + ul_filter_160M_freq_star);
    freqStopRF = uint64_t(2e9 + ul_filter_160M_freq_stop);
    freqStepRF = uint64_t(ul_filter_freq_step);
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        m_rx_rf_fr->get(freqRF,&tempDataRFFr_160[idx]);
        idx ++;
    }

    idx = 0;
    map2buf(cal_file::RX_IF_FR);
    freqStarIF = ul_filter_160M_freq_star;
    freqStopIF = ul_filter_160M_freq_stop;
    freqStepIF = ul_filter_freq_step;
    for (freqIF = freqStarIF;freqIF <= freqStopIF;freqIF += freqStepIF) {
        m_rx_if_fr->get(freqIF,&tempDataIFFr_160[idx]);
        idx ++;
    }

    // Real IF frequency response,-83M~~~83M.
    for (idx = 0;idx < ul_filter_160M_freqs;idx ++) {
        tempDataIFFr_160[idx].pwr -= tempDataRFFr_160[idx].pwr;
    }

    map2buf(cal_file::RX_RF_FR);
    freqStarRF = rx_freq_star;
    freqStopRF = rx_freq_stop;
    freqStepRF = r1c::rx_freq_step_called;
    freqCalled = 0;
    for (freqRF = freqStarRF;freqRF <= freqStopRF;freqRF += freqStepRF) {
        freqCalled = freqRF < 100000000 ? 100000000 : freqRF;
        idx = 0;
        freqStarRF_InBw = uint64_t(freqCalled + ul_filter_160M_freq_star);
        freqStopRF_InBw = uint64_t(freqCalled + ul_filter_160M_freq_stop);
        freqStepRF_InBw = uint64_t(ul_filter_freq_step);
        for (uint64_t freq = freqStarRF_InBw;freq <= freqStopRF_InBw;freq += freqStepRF_InBw) {
            m_rx_rf_fr->get(freq,&tempDataRFFr_160[idx]);
            fprintf(fp_fr,"%.6f ",tempDataRFFr_160[idx].pwr + tempDataIFFr_160[idx].pwr);
            idx ++;
        }
        fprintf(fp_fr,"\n");
    }
    fclose(fp_fr);
    fp_fr = nullptr;
    return 0;
}

int32_t cal_file_r1cd::gen_rx_filter_coef()
{
    string path = boost::filesystem::initial_path<boost::filesystem::path>().string()
                  + "/" + "rx_filter.exe";

    if (fir_process(path)) {
        return -1;
    }
    return 0;
}

int32_t cal_file_r1cd::update_rx_filter_table()
{
    char path_real[256];
    char path_imag[256];

    FILE *fp_real = nullptr;
    FILE *fp_imag = nullptr;

    rx_filter_80m_table::data_f_t  tempData_80;
    rx_filter_160m_table::data_f_t tempData_160;

    uint32_t idx = 0;

    // 80M
    sprintf(path_real,"C:\\CSECal\\rx_filter_80_coef_real.txt");
    sprintf(path_imag,"C:\\CSECal\\rx_filter_80_coef_imag.txt");
    fp_real = fopen(path_real,"r");
    fp_imag = fopen(path_imag,"r");
    if (fp_real == nullptr) {
        Log.set_last_err("Could not open %s",path_real);
        return -1;
    }
    if (fp_imag == nullptr) {
        Log.set_last_err("Could not open %s",path_imag);
        return -1;
    }

    for (uint64_t freq = rx_freq_star;freq <= rx_freq_stop;freq += r1c::rx_freq_step_called) {
        ZERO_OBJ(tempData_80);
        for (uint32_t i = 0;i < ul_filter_tap;i ++) {
            fscanf(fp_real,"%hd,",&(tempData_80.coef_real[i]));
        }
        fscanf(fp_real,"\n");

        for (uint32_t i = 0;i < ul_filter_tap;i ++) {
            fscanf(fp_imag,"%hd,",&(tempData_80.coef_imag[i]));
        }
        fscanf(fp_imag,"\n");

        tempData_80.freq = freq;
        tempData_80.time = Log.time_current();

        add(cal_file::RX_FILTER_80,&tempData_80);
        idx ++;
    }
    w(cal_file::RX_FILTER_80);
    fclose(fp_real);
    fclose(fp_imag);
    fp_real = nullptr;
    fp_imag = nullptr;

#ifdef RD_PLATFORM_WIN32
    DeleteFileA(path_real);
    DeleteFileA(path_imag);
#else
#endif

    // 160M
    sprintf(path_real,"C:\\CSECal\\rx_filter_160_coef_real.txt");
    sprintf(path_imag,"C:\\CSECal\\rx_filter_160_coef_imag.txt");
    fp_real = fopen(path_real,"r");
    fp_imag = fopen(path_imag,"r");
    if (fp_real == nullptr) {
        Log.set_last_err("Could not open %s",path_real);
        return -1;
    }
    if (fp_imag == nullptr) {
        Log.set_last_err("Could not open %s",path_imag);
        return -1;
    }

    idx = 0;
    for (uint64_t freq = rx_freq_star;freq <= rx_freq_stop;freq += r1c::rx_freq_step_called) {
        ZERO_OBJ(tempData_160);
        for (uint32_t i = 0;i < ul_filter_tap;i ++) {
            fscanf(fp_real,"%hd,",&(tempData_160.coef_real[i]));
        }
        fscanf(fp_real,"\n");

        for (uint32_t i = 0;i < ul_filter_tap;i ++) {
            fscanf(fp_imag,"%hd,",&(tempData_160.coef_imag[i]));
        }
        fscanf(fp_imag,"\n");

        tempData_160.freq = freq;
        tempData_160.time = Log.time_current();

        add(cal_file::RX_FILTER_160,&tempData_160);
        idx ++;
    }
    w(cal_file::RX_FILTER_160);
    fclose(fp_real);
    fclose(fp_imag);
    fp_real = nullptr;
    fp_imag = nullptr;

#ifdef RD_PLATFORM_WIN32
    DeleteFileA(path_real);
    DeleteFileA(path_imag);
#else
#endif

    return 0;
}
