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
using namespace rd;
using namespace ns_sp1401;
using namespace rd::ns_sp2401;

cal_file_r1cd::cal_file_r1cd(hw_ver_t ver, uint32_t rf_idx, uint32_t rfu_idx) :
    cal_file(ver,rf_idx,rfu_idx),
    m_tx_lol(nullptr),
    m_tx_sb(nullptr),
    m_tx_pwr_op(nullptr),
    m_tx_pwr_io(nullptr),
    m_tx_att_op(nullptr),
    m_tx_att_io(nullptr),
    m_rx_ref_op(nullptr),
    m_rx_ref_io(nullptr),
    m_rx_att_op(nullptr),
    m_rx_att_io(nullptr),
    m_tx_filter_80m(nullptr),
    m_tx_filter_160m(nullptr),
    m_rx_filter_80m(nullptr),
    m_rx_filter_160m(nullptr),
    m_tx_filter_offset_op_80m(nullptr),
    m_tx_filter_offset_io_80m(nullptr),
    m_rx_filter_offset_op_80m(nullptr),
    m_rx_filter_offset_io_80m(nullptr),
    m_tx_tc(nullptr),
    m_rx_tc(nullptr),
    m_tx_rf_fr_0(nullptr),
    m_tx_rf_fr_1(nullptr),
    m_tx_if_fr(nullptr),
    m_rx_rf_fr(nullptr),
    m_rx_if_fr(nullptr),
    _tx_tp(nullptr),
    _rx_tp(nullptr)
{
}

cal_file_r1cd::~cal_file_r1cd()
{
}

int32_t cal_file_r1cd::open()
{
    BOOST_SPTR_SAFE_MAKE(x9119_table,g_cal_table_x9119);
    BOOST_SPTR_SAFE_MAKE(tx_lol_table_r1cd,m_tx_lol);
    BOOST_SPTR_SAFE_MAKE(tx_sb_table_r1cd,m_tx_sb);
    BOOST_SPTR_SAFE_MAKE(tx_pwr_op_table_r1c,m_tx_pwr_op);
    BOOST_SPTR_SAFE_MAKE(tx_pwr_io_table_r1c,m_tx_pwr_io);
    BOOST_SPTR_SAFE_MAKE(tx_att_op_table_r1cd,m_tx_att_op);
    BOOST_SPTR_SAFE_MAKE(tx_att_io_table_r1cd,m_tx_att_io);
    BOOST_SPTR_SAFE_MAKE(rx_ref_op_table_r1cd,m_rx_ref_op);
    BOOST_SPTR_SAFE_MAKE(rx_ref_io_table_r1cd,m_rx_ref_io);
    BOOST_SPTR_SAFE_MAKE(rx_att_op_table_r1cd,m_rx_att_op);
    BOOST_SPTR_SAFE_MAKE(rx_att_io_table_r1cd,m_rx_att_io);
    BOOST_SPTR_SAFE_MAKE(tx_tc_table,m_tx_tc);
    BOOST_SPTR_SAFE_MAKE(rx_tc_table,m_rx_tc);

    if (_80M == m_bw) {
        BOOST_SPTR_SAFE_MAKE(tx_filter_80m_table,m_tx_filter_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_80m_table,m_rx_filter_80m);
        BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_op_80m);
        BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_io_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_op_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_io_80m);
	}
    else if (_160M == m_bw) {
        BOOST_SPTR_SAFE_MAKE(tx_filter_160m_table,m_tx_filter_160m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_160m_table,m_rx_filter_160m);
	}

    return cal_file::open();
}

void cal_file_r1cd::set_bw(bw_t bw)
{
    if (m_bw == bw)
        return;
	if (_80M == bw) {
        m_tx_filter_160m.reset();
        m_rx_filter_160m.reset();

        BOOST_SPTR_SAFE_MAKE(tx_filter_80m_table,m_tx_filter_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_80m_table,m_rx_filter_80m);
        BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_op_80m);
        BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_io_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_op_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_io_80m);

        MAP_FILE_DATAM(TX_FILTER_80,m_tx_filter_80m);
        MAP_FILE_DATAM(RX_FILTER_80,m_rx_filter_80m);
        MAP_FILE_DATAM(TX_FILTER_OFFSET_OP_80,m_tx_filter_offset_op_80m);
        MAP_FILE_DATAM(TX_FILTER_OFFSET_IO_80,m_tx_filter_offset_io_80m);
        MAP_FILE_DATAM(RX_FILTER_OFFSET_OP_80,m_rx_filter_offset_op_80m);
        MAP_FILE_DATAM(RX_FILTER_OFFSET_IO_80,m_rx_filter_offset_io_80m);
	}
	else if (_160M == bw) {
        m_tx_filter_offset_op_80m.reset();
        m_tx_filter_offset_io_80m.reset();
        m_rx_filter_offset_op_80m.reset();
        m_rx_filter_offset_io_80m.reset();
        m_tx_filter_80m.reset();
        m_rx_filter_80m.reset();

        BOOST_SPTR_SAFE_MAKE(tx_filter_160m_table,m_tx_filter_160m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_160m_table,m_rx_filter_160m);

        MAP_FILE_DATAM(TX_FILTER_160,m_tx_filter_160m);
        MAP_FILE_DATAM(RX_FILTER_160,m_rx_filter_160m);
	}
    return cal_file::set_bw(bw);
}

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

int32_t cal_file_r1cd::add(cal_item_t item,void *data)
{
    map2buf(item);
    switch (item) {
    case X9119         : ADD_DATAF(g_cal_table_x9119,x9119_table::data_t);
    case TX_LOL        : ADD_DATAF(m_tx_lol,tx_lol_table_r1cd::data_f_t);
    case TX_SB         : ADD_DATAF(m_tx_sb,tx_sb_table_r1cd::data_f_t);
    case TX_PWR_OP     : ADD_DATAF(m_tx_pwr_op,tx_pwr_op_table_r1c::data_f_t);
    case TX_PWR_IO     : ADD_DATAF(m_tx_pwr_io,tx_pwr_io_table_r1c::data_f_t);
    case TX_ATT_OP     : ADD_DATAF(m_tx_att_op,tx_att_op_table_r1cd::data_f_t);
    case TX_ATT_IO     : ADD_DATAF(m_tx_att_io,tx_att_io_table_r1cd::data_f_t);
    case RX_REF_OP     : ADD_DATAF(m_rx_ref_op,rx_ref_op_table_r1cd::data_f_t);
    case RX_REF_IO     : ADD_DATAF(m_rx_ref_io,rx_ref_io_table_r1cd::data_f_t);
    case RX_ATT_OP     : ADD_DATAF(m_rx_att_op,rx_att_op_table_r1cd::data_f_t);
    case RX_ATT_IO     : ADD_DATAF(m_rx_att_io,rx_att_io_table_r1cd::data_f_t);
    case TX_FILTER_80  : ADD_DATAF(m_tx_filter_80m,tx_filter_80m_table::data_f_t);
    case TX_FILTER_160 : ADD_DATAF(m_tx_filter_160m,tx_filter_160m_table::data_f_t);
    case RX_FILTER_80  : ADD_DATAF(m_rx_filter_80m,rx_filter_80m_table::data_f_t);
    case RX_FILTER_160 : ADD_DATAF(m_rx_filter_160m,rx_filter_160m_table::data_f_t);
    case TX_FILTER_OFFSET_OP_80 : ADD_DATAF(m_tx_filter_offset_op_80m,tx_filter_offset_table::data_f_t);
    case TX_FILTER_OFFSET_IO_80 : ADD_DATAF(m_tx_filter_offset_io_80m,tx_filter_offset_table::data_f_t);
    case RX_FILTER_OFFSET_OP_80 : ADD_DATAF(m_rx_filter_offset_op_80m,rx_filter_offset_table::data_f_t);
    case RX_FILTER_OFFSET_IO_80 : ADD_DATAF(m_rx_filter_offset_io_80m,rx_filter_offset_table::data_f_t);
    case TX_TC : ADD_DATAF(m_tx_tc,tx_tc_table::data_f_t);
    case RX_TC : ADD_DATAF(m_rx_tc,rx_tc_table::data_f_t);

    case TX_RF_FR_0 : ADD_DATAF(m_tx_rf_fr_0,tx_rf_fr_0_table_r1cd::data_f_t);
    case TX_RF_FR_1 : ADD_DATAF(m_tx_rf_fr_1,tx_rf_fr_1_table_r1cd::data_f_t);
    case TX_IF_FR   : ADD_DATAF(m_tx_if_fr,tx_if_fr_table::data_f_t);
    case RX_RF_FR   : ADD_DATAF(m_rx_rf_fr,rx_rf_fr_table::data_f_t);
    case RX_IF_FR   : ADD_DATAF(m_rx_if_fr,rx_if_fr_table::data_f_t);
    case TX_TP      : ADD_DATAF(_tx_tp,tx_tp_table::data_f_t);
    case RX_TP      : ADD_DATAF(_rx_tp,rx_tp_table::data_f_t);
    default:return 0;
	}
	return 0;
}

int32_t cal_file_r1cd::map2buf(cal_item_t item)
{
    if (nullptr == g_cal_item_buf.buf) {
        cal_file::file_info_t Info;
        uint32_t max_byte = cal_file::get_max_item_byte();

        g_cal_item_buf.buf = new char[max_byte / sizeof(char)];
        if (nullptr == g_cal_item_buf.buf) {
            Log.set_last_err("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
			return -1;
		}
        memset(g_cal_item_buf.buf,0,max_byte);
	}

    if (item != g_cal_item_buf.item || this->m_rf_idx != g_cal_item_buf.rf_idx || this->m_rfu_idx != g_cal_item_buf.rfu_idx) {
        INT_CHECK(r(item,g_cal_item_buf.buf));
        g_cal_item_buf.item = item;
        g_cal_item_buf.rf_idx = this->m_rf_idx;
        g_cal_item_buf.rfu_idx = this->m_rfu_idx;
        switch (item) {
        case X9119     : MAP_GLOBAL_DATAF(g_cal_table_x9119,x9119_table::data_t);
        case TX_LOL    : MAP_GLOBAL_DATAF(m_tx_lol,tx_lol_table_r1cd::data_f_t );
        case TX_SB     : MAP_GLOBAL_DATAF(m_tx_sb,tx_sb_table_r1cd::data_f_t);
        case TX_PWR_OP : MAP_GLOBAL_DATAF(m_tx_pwr_op,tx_pwr_op_table_r1c::data_f_t);
        case TX_PWR_IO : MAP_GLOBAL_DATAF(m_tx_pwr_io,tx_pwr_io_table_r1c::data_f_t);
        case TX_ATT_OP : MAP_GLOBAL_DATAF(m_tx_att_op,tx_att_op_table_r1cd::data_f_t);
        case TX_ATT_IO : MAP_GLOBAL_DATAF(m_tx_att_io,tx_att_io_table_r1cd::data_f_t);
        case RX_REF_OP : MAP_GLOBAL_DATAF(m_rx_ref_op,rx_ref_op_table_r1cd::data_f_t);
        case RX_REF_IO : MAP_GLOBAL_DATAF(m_rx_ref_io,rx_ref_io_table_r1cd::data_f_t);
        case RX_ATT_OP : MAP_GLOBAL_DATAF(m_rx_att_op,rx_att_op_table_r1cd::data_f_t);
        case RX_ATT_IO : MAP_GLOBAL_DATAF(m_rx_att_io,rx_att_io_table_r1cd::data_f_t);
        case TX_TC     : MAP_GLOBAL_DATAF(m_tx_tc,tx_tc_table::data_f_t);
        case RX_TC     : MAP_GLOBAL_DATAF(m_rx_tc,rx_tc_table::data_f_t);

        case TX_FILTER_80 : {
            BOOST_SPTR_SAFE_MAKE(tx_filter_80m_table,m_tx_filter_80m);
            MAP_GLOBAL_DATAF(m_tx_filter_80m,tx_filter_80m_table::data_f_t);
        }
        case TX_FILTER_160 : {
            BOOST_SPTR_SAFE_MAKE(tx_filter_160m_table,m_tx_filter_160m);
            MAP_GLOBAL_DATAF(m_tx_filter_160m,tx_filter_160m_table::data_f_t);
        }
        case RX_FILTER_80 : {
            BOOST_SPTR_SAFE_MAKE(rx_filter_80m_table,m_rx_filter_80m);
            MAP_GLOBAL_DATAF(m_rx_filter_80m,rx_filter_80m_table::data_f_t);
        }
        case RX_FILTER_160 : {
            BOOST_SPTR_SAFE_MAKE(rx_filter_160m_table,m_rx_filter_160m);
            MAP_GLOBAL_DATAF(m_rx_filter_160m,rx_filter_160m_table::data_f_t);
        }
        case TX_FILTER_OFFSET_OP_80 : {
            BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_op_80m);
            MAP_GLOBAL_DATAF(m_tx_filter_offset_op_80m,tx_filter_offset_table::data_f_t);
        }
        case TX_FILTER_OFFSET_IO_80 : {
            BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_io_80m);
            MAP_GLOBAL_DATAF(m_tx_filter_offset_io_80m,tx_filter_offset_table::data_f_t);
        }
        case RX_FILTER_OFFSET_OP_80 : {
            BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_op_80m);
            MAP_GLOBAL_DATAF(m_rx_filter_offset_op_80m,rx_filter_offset_table::data_f_t);
        }
        case RX_FILTER_OFFSET_IO_80 : {
            BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_io_80m);
            MAP_GLOBAL_DATAF(m_rx_filter_offset_io_80m,rx_filter_offset_table::data_f_t);
        }
        case TX_RF_FR_0 : {
            BOOST_SPTR_SAFE_MAKE(tx_rf_fr_0_table_r1cd,m_tx_rf_fr_0);
            MAP_GLOBAL_DATAF(m_tx_rf_fr_0,tx_rf_fr_table::data_f_t);
        }
        case TX_RF_FR_1 : {
            BOOST_SPTR_SAFE_MAKE(tx_rf_fr_1_table_r1cd,m_tx_rf_fr_1);
            MAP_GLOBAL_DATAF(m_tx_rf_fr_1,tx_rf_fr_table::data_f_t);
        }
        case TX_IF_FR : {
            BOOST_SPTR_SAFE_MAKE(tx_if_fr_table,m_tx_if_fr);
            MAP_GLOBAL_DATAF(m_tx_if_fr,tx_if_fr_table::data_f_t);
        }
        case RX_RF_FR : {
            BOOST_SPTR_SAFE_MAKE(rx_rf_fr_table,m_rx_rf_fr);
            MAP_GLOBAL_DATAF(m_rx_rf_fr,rx_rf_fr_table::data_f_t);
        }
        case RX_IF_FR : {
            BOOST_SPTR_SAFE_MAKE(rx_if_fr_table,m_rx_if_fr);
            MAP_GLOBAL_DATAF(m_rx_if_fr,rx_if_fr_table::data_f_t);
        }
        case TX_TP : {
            BOOST_SPTR_SAFE_MAKE(tx_tp_table,_tx_tp);
            MAP_GLOBAL_DATAF(_tx_tp,tx_tp_table::data_f_t);
        }
        case RX_TP : {
            BOOST_SPTR_SAFE_MAKE(rx_tp_table,_rx_tp);
            MAP_GLOBAL_DATAF(_rx_tp,rx_tp_table::data_f_t);
        }
        default:return 0;
		}
	}
	return 0;
}

int32_t cal_file_r1cd::map2mem()
{
    MAP_FILE_DATAM(X9119,g_cal_table_x9119);
    MAP_FILE_DATAM(TX_LOL,m_tx_lol);
    MAP_FILE_DATAM(TX_SB,m_tx_sb);
    MAP_FILE_DATAM(TX_PWR_OP,m_tx_pwr_op);
    MAP_FILE_DATAM(TX_PWR_IO,m_tx_pwr_io);
    MAP_FILE_DATAM(TX_ATT_OP,m_tx_att_op);
    MAP_FILE_DATAM(TX_ATT_IO,m_tx_att_io);
    MAP_FILE_DATAM(RX_REF_OP,m_rx_ref_op);
    MAP_FILE_DATAM(RX_REF_IO,m_rx_ref_io);
    MAP_FILE_DATAM(RX_ATT_OP,m_rx_att_op);
    MAP_FILE_DATAM(RX_ATT_IO,m_rx_att_io);
//    MAP_FILE_DATAM(TX_TC,m_tx_tc);
//    MAP_FILE_DATAM(RX_TC,m_rx_tc);
    if (_80M == m_bw) {
        MAP_FILE_DATAM(TX_FILTER_80,m_tx_filter_80m);
        MAP_FILE_DATAM(TX_FILTER_OFFSET_OP_80,m_tx_filter_offset_op_80m);
        MAP_FILE_DATAM(TX_FILTER_OFFSET_IO_80,m_tx_filter_offset_io_80m);
        MAP_FILE_DATAM(RX_FILTER_80,m_rx_filter_80m);
        MAP_FILE_DATAM(RX_FILTER_OFFSET_OP_80,m_rx_filter_offset_op_80m);
        MAP_FILE_DATAM(RX_FILTER_OFFSET_IO_80,m_rx_filter_offset_io_80m);
    } else if (_160M == m_bw) {
        MAP_FILE_DATAM(TX_FILTER_160,m_tx_filter_160m);
        MAP_FILE_DATAM(RX_FILTER_160,m_rx_filter_160m);
	}
    return 0;
}

int32_t cal_file_r1cd::create()
{
    cal_file::file_ver_t ver;
    cal_file::file_info_t r1ab_info;
    cal_file::file_info_t r1cd_info;
    uint32_t item_idx_r1ab = 0;
    uint32_t item_idx_r1cd = 0;
    uint32_t max_byte_r1ab = 0;
    uint32_t max_byte_r1cd = 0;
    uint32_t max_byte = 0;
    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",m_rfu_idx,m_rf_idx);

    ver.head = CAL_FILE_VER_HEAD;
    ver.ver  = CAL_FILE_VER;
    ver.tail = CAL_FILE_VER_TAIL;

    r1ab_info.head = CAL_FILE_INFO_HEAD;
    r1ab_info.tail = CAL_FILE_INFO_TAIL;
    memset(r1ab_info.sn,0,sizeof(r1ab_info.sn));
    sprintf(r1ab_info.sn,"SP1401R1B%08dRFU%uRF%u",m_ordinal,m_rfu_idx,m_rf_idx);

    max_byte_r1ab = cal_file_r1ab::get_fix_item_table_r1ab(r1ab_info.item_info,r1ab_info.items);
    max_byte_r1cd = cal_file_r1cd::get_fix_item_table_r1cd(r1cd_info.item_info,r1cd_info.items);
    max_byte = max_byte_r1ab >= max_byte_r1cd ? max_byte_r1ab : max_byte_r1cd;

    r1ab_info.items -= 1;
    for (uint32_t i = 0;i < r1ab_info.items;i ++) {
		if (X9119 == i)		//r1a's cal item table = enum CalItem,so it is safe here
            item_idx_r1ab ++;
        r1ab_info.item_info[i] = r1ab_info.item_info[item_idx_r1ab];
        item_idx_r1ab ++;
	}

    FILE *fp;
    if (access("C:\\CSECal",0) == -1) {
#ifdef PLATFORM_WIN32
        if (mkdir("C:\\CSECal") == -1) {
#elif defined PLATFORM_LINUX
        if (mkdir("/var/lib/CSECal",0777) == -1) {
#endif
            Log.set_last_err("%s:%s:%d(%s)",__FILE__,__FUNCTION__,__LINE__,strerror(errno));
            return -1;
        }
    }
    if (access("C:\\CSECal\\cxu.cal",0) == -1) {
        x9119_table::data_t data;
        data.value = 0;

        fp = fopen("C:\\CSECal\\cxu.cal","w+");
        CFO_ASSERT(fp,nullptr == fp);
        CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
        CFO_ASSERT(fp,fwrite(&data,sizeof(x9119_table::data_t),1,fp) == 0);
        CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
        fclose(fp);
    }
    //r1a/b
    fp = fopen(path,"w+");
    CFO_ASSERT(fp,nullptr == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&ver,sizeof(cal_file::file_ver_t),1,fp) == 0);
    CFO_ASSERT(fp,fwrite(&r1ab_info,sizeof(cal_file::file_info_t),1,fp) == 0);
    for (uint32_t i = 0;i < r1ab_info.items;i ++)
        CFO_ASSERT(fp,fwrite(&(r1ab_info.item_info[i]),sizeof(cal_file::item_info_t),1,fp) == 0);

    unsigned char *buf = new unsigned char[max_byte];
    for (uint32_t i = 0;i < r1ab_info.items;i ++) {
        uint32_t size = ((r1ab_info.item_info[i]).size) * ((r1ab_info.item_info[i]).pt);
        memset(buf,0,max_byte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(buf,1,size,fp) == 0);
    }

    //r1c/d
    r1cd_info.head = CAL_FILE_INFO_HEAD;
    r1cd_info.tail = CAL_FILE_INFO_TAIL;
    memset(r1cd_info.sn,0,sizeof(r1cd_info.sn));
    sprintf(r1cd_info.sn,"SP1401R1C%08dRFU%uRF%u",m_ordinal,m_rfu_idx,m_rf_idx);

    r1cd_info.items -= 1;
    for (uint32_t i = 0;i < r1cd_info.items;i ++) {
        if (X9119 == r1cd_info.item_info[item_idx_r1cd].item)	//r1c may add or only use a part of cal item which defined before
            item_idx_r1cd ++;
        r1cd_info.item_info[i] = r1cd_info.item_info[item_idx_r1cd];
        item_idx_r1cd ++;
	}
	
    CFO_ASSERT(fp,fwrite(&r1cd_info,sizeof(cal_file::file_info_t),1,fp) == 0);
    for (uint32_t i = 0;i < r1cd_info.items;i ++)
        CFO_ASSERT(fp,fwrite(&(r1cd_info.item_info[i]),sizeof(cal_file::item_info_t),1,fp) == 0);

    for (uint32_t i = 0;i < r1cd_info.items;i ++) {
        uint32_t size = ((r1cd_info.item_info[i]).size) * ((r1cd_info.item_info[i]).pt);
        memset(buf,0,max_byte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(buf,1,size,fp) == 0);
	}
    delete []buf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t cal_file_r1cd::load(file_info_t &info)
{
    file_ver_t ver;
    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",m_rfu_idx,m_rf_idx);

    FILE *fp = fopen(path,"rb");
    CFO_ASSERT(fp,nullptr == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&ver,sizeof(file_ver_t),1,fp) == 0);
    show_file_ver(&ver);

    CFO_ASSERT(fp,fseek(fp,R1A_FILE_SIZE,SEEK_SET));

    item_info_t *item_info = info.item_info;
    CFO_ASSERT(fp,fread(&info,sizeof(cal_file::file_info_t),1,fp) == 0);
    info.item_info = item_info;
    show_file_info(&info);

    for (uint32_t i = 0;i < info.items;i ++) {
        CFO_ASSERT(fp,fread(&(info.item_info[i]),sizeof(cal_file::item_info_t),1,fp) == 0);
        show_item_info(&(info.item_info[i]));
	}
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t cal_file_r1cd::get_item_size(cal_item_t item,uint32_t &pos,uint32_t &size)
{
    cal_file::file_info_t info;
    INT_CHECK(load(info));

    uint32_t pt = 0;
    for (uint32_t i = 0;i < info.items;i ++) {
        if (info.item_info[i].item == item) {
            size = info.item_info[i].size;
            pt = info.item_info[i].pt;
			break;
		}
	}
    size = size * pt;

    if (cal_file::X9119 == item) {
        pos = 0;
        size = sizeof(x9119_table::data_t);
		return 0;
	}

    pos = R1A_FILE_SIZE;
    pos += sizeof(cal_file::file_info_t);
    for (uint32_t i = 0;i < info.items;i ++) {
        pos += sizeof(cal_file::item_info_t);
    }
    for (uint32_t i = 0;i < info.items;i ++) {
        if (item != info.item_info[i].item) {
            pos += ((info.item_info[i]).size) * ((info.item_info[i]).pt);
        } else {
			break;
        }
	}
	return 0;
}
