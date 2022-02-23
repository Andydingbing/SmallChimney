#include "sp1401.h"
#include "../chip/reg_def_sp9500.h"
#include "sleep_common.h"
#include "algo_math.hpp"
#include "ftp.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>

using namespace std;
using namespace rd;
using namespace rd::ns_sp9500;
using namespace ns_sp1401;
using namespace boost::gregorian;
using namespace boost::filesystem;

const double sp1401::temp_min = 0.0;
const double sp1401::temp_max = 100.0;
const double sp1401::temp_granularity = 0.1;
const uint32_t sp1401::temps = 1001;

boost::function<bool()> sp1401::_ftp_retry_call_back = boost::function<bool()>();

sp1401::sp1401()
{
    m_k7 = nullptr;
    m_arb_reader = boost::make_shared<arb_reader>();
    en_tx_tc = false;
    en_rx_tc = false;
}

int32_t sp1401::open_board()
{
    INT_CHECK(set_iq_cap_src(IQ_CAP_SRC_INT_TRIG,false));
    INT_CHECK(set_iq_cap_timeout(6553600));
    INT_CHECK(set_iq_cap_trig_threshold(0.0));
    return 0;
}

int32_t sp1401::get_hw_ver(const char *sn,hw_ver_t &ver)
{
    if (sn[6] == 'R' && sn[7] == '1') {
        if      (sn[8] == 'A') { ver = R1A; }
        else if (sn[8] == 'B') { ver = R1B; }
        else if (sn[8] == 'C') { ver = R1C; }
        else if (sn[8] == 'D') { ver = R1D; }
        else if (sn[8] == 'E') { ver = R1E; }
        else if (sn[8] == 'F') { ver = R1F; }
        else                   { ver = HW_ERROR; return -1; }
    } else {
        ver = HW_ERROR;
        return -1;
    }
    return 0;
}

string sp1401::hw_ver2str(hw_ver_t ver)
{
    switch (ver) {
        case HW_ERROR : return string("HW_ERROR");
        case R1A      : return string("R1A");
        case R1B      : return string("R1B");
        case R1C      : return string("R1C");
        case R1D      : return string("R1D");
        case R1E      : return string("R1E");
        case R1F      : return string("R1F");
        case HW_VER_SP9500_MAX   : return string("unknown version");
    }
    return string("unknown version");
}

string sp1401::hw_ver2sn_header(hw_ver_t ver)
{
    switch (ver) {
        case HW_ERROR : return string("SP1401ERR");
        case R1A : return string("SP1401R1A");
        case R1B : return string("SP1401R1B");
        case R1C : return string("SP1401R1C");
        case R1D : return string("SP1401R1D");
        case R1E : return string("SP1401R1E");
        case R1F : return string("SP1401R1F");
        case HW_VER_SP9500_MAX : return string("SP1401ERR");
    }
    return string("SP1401ERR");
}

int32_t sp1401::get_k7_ver(uint32_t &ver)
{
    RFU_K7_REG_DECLARE(0x0000);
    RFU_K7_R(0x0000);
    ver = RFU_K7_REG_DATA(0x0000);
    return 0;
}

int32_t sp1401::set_sn_major(const hw_ver_t &ver,const uint32_t &ordinal)
{
    char sn[32] = {0};
    ZERO_ARRAY(sn);
    uint32_t rf_idx = m_cal_file->rf_idx();
    uint32_t rfu_idx = m_cal_file->rfu_idx();
    sprintf(sn,"%9s%08XRFU%dRF%d",
            hw_ver2sn_header(ver).c_str(),
            ass_ordinal(ordinal),
            rfu_idx,
            rf_idx);
    INT_CHECK(w_eeprom(0x0000,g_sn_length,sn));
    return 0;
}

int32_t sp1401::set_sn_redundant(const sn_redundant_t &info,
                                       const vector<tx_trouble_shoot_t> &tx_ts,
                                       const vector<rx_trouble_shoot_t> &rx_ts,
                                       const string &remark)
{
    if (info.tx_ts_num != tx_ts.size()) {
        return -1;
    }

    if (info.rx_ts_num != rx_ts.size()) {
        return -1;
    }

    uint16_t cur_pos = 0x0000 + g_sn_length;
    INT_CHECK(w_eeprom(cur_pos,sizeof(info),(char*)(&info)));
    cur_pos += sizeof(info);

    for (uint32_t i = 0;i < info.tx_ts_num;i ++) {
        INT_CHECK(w_eeprom(cur_pos,sizeof(tx_trouble_shoot_t),(char *)(&tx_ts[i])));
        cur_pos += sizeof(tx_trouble_shoot_t);
    }
    for (uint32_t i = 0;i < info.rx_ts_num;i ++) {
        INT_CHECK(w_eeprom(cur_pos,sizeof(rx_trouble_shoot_t),(char *)(&rx_ts[i])));
        cur_pos += sizeof(rx_trouble_shoot_t);
    }
    INT_CHECK(w_eeprom(cur_pos,remark.length(),remark.c_str()));
    return 0;
}

int32_t sp1401::get_sn_redundant(sn_redundant_t &info,
                                       vector<tx_trouble_shoot_t> &tx_ts,
                                       vector<rx_trouble_shoot_t> &rx_ts,
                                       string &remark)
{
    tx_ts.clear();
    rx_ts.clear();

    uint64_t cur_pos = 0x0000 + g_sn_length;
    INT_CHECK(r_eeprom(cur_pos,sizeof(info),(char *)(&info)));
    cur_pos += sizeof(info);

    tx_trouble_shoot_t each_tx_ts;
    rx_trouble_shoot_t each_rx_ts;
    char str_remark[512];
    ZERO_ARRAY(str_remark);

    for (uint32_t i = 0;i < info.tx_ts_num;i ++) {
        INT_CHECK(r_eeprom(cur_pos,sizeof(tx_trouble_shoot_t),(char *)(&each_tx_ts)));
        tx_ts.push_back(each_tx_ts);
        cur_pos += sizeof(tx_trouble_shoot_t);
    }
    for (uint32_t i = 0;i < info.rx_ts_num;i ++) {
        INT_CHECK(r_eeprom(cur_pos,sizeof(rx_trouble_shoot_t),(char *)(&each_rx_ts)));
        rx_ts.push_back(each_rx_ts);
        cur_pos += sizeof(rx_trouble_shoot_t);
    }
    INT_CHECK(r_eeprom(cur_pos,info.remark_num,str_remark));
    for (uint32_t i = 0;i < info.remark_num;i ++)
        remark.push_back(str_remark[i]);
    return 0;
}

int32_t sp1401::get_sn_major(char *sn)
{
    INT_CHECK(r_eeprom(0x0000,24,sn));
    return 0;
}

uint32_t sp1401::ass_ordinal(uint32_t ordinal)
{
    uint32_t assed_ordinal = 0;
    days days_elapsed = day_clock::local_day() - date(2019,1,1);
    days_elapsed = days_elapsed < days(0) ? days(0) : days_elapsed;
    assed_ordinal = (uint32_t(days_elapsed.days() << 16) | (ordinal & 0xFFFF));
    return assed_ordinal;
}

int32_t sp1401::is_valid_sn(const char *sn)
{
    int32_t i = 0;

    if (!strcmp(sn,"")) {
        return SN_NULL;
    }

    for (i = 0;i < g_sn_length;i ++) {
        if (sn[i] != '0') {
            break;
        }
    }

    if (g_sn_length - 1 == i) {
        return SN_NULL;
    }

    bool is_valid = false;
    char sn_hw_ver[8],sn_slot[8],sn_slot_temp[8];

    for (uint32_t i = 0;i < sizeof(sn_hw_ver);i ++) {
        sn_hw_ver[i] = sn[i];
    }
    for (uint32_t i = 0;i < sizeof(sn_slot);i ++) {
        sn_slot[i] = sn[17 + i];
    }
    memset(sn_slot_temp,0,sizeof(sn_slot_temp));

    if (!strstr(sn_hw_ver,"SP1401R1")) {
        Log.set_last_err("sn err:%24s",sn);
        return SN_WRONG;
    }
    for (int i = 0;i < g_max_rfu;i ++) {
        for (int j = 0;j < g_max_rf;j ++) {
            sprintf(sn_slot_temp,"RFU%dRF%d",i,j);
            if (strstr(sn_slot,sn_slot_temp)) {
                is_valid = true;
                break;
            }
        }
        if (is_valid) {
            break;
        }
    }
    if (is_valid == false) {
        Log.set_last_err("sn err:%24s",sn);
        return SN_WRONG;
    }
    return SN_RIGHT;
}

int32_t sp1401::set_arb_segments(uint16_t segs)
{
    RFU_K7_REG_DECLARE_2(0x0505,0x0605);
    RFU_K7_REG_2(0x0505,0x0605).seg = segs;
    RFU_K7_W_2(0x0505,0x0605);
    return 0;
}

int32_t sp1401::set_arb_param(uint32_t add_samp,uint32_t cycle,uint32_t rep_mode)
{
    RFU_K7_REG_DECLARE_2(0x0502,0x0602);
    RFU_K7_REG_2(0x0502,0x0602).addition   = add_samp;
    RFU_K7_REG_2(0x0502,0x0602).cycles     = cycle;
    RFU_K7_REG_2(0x0502,0x0602).repetition = rep_mode;
    RFU_K7_W_2(0x0502,0x0602);
    return 0;
}

int32_t sp1401::set_arb_trigger(bool retrigger, bool auto_start, uint32_t src, uint32_t trig_delay)
{
    RFU_K7_REG_DECLARE_2(0x050a,0x060a);
    RFU_K7_REG_2(0x050a,0x060a).retrig    = retrigger;
    RFU_K7_REG_2(0x050a,0x060a).delay     = trig_delay;
    RFU_K7_REG_2(0x050a,0x060a).autostart = auto_start;
    RFU_K7_REG_2(0x050a,0x060a).src       = src;
    RFU_K7_W_2(0x050a,0x060a);
    return 0;
}

int32_t sp1401::set_arb_manual_trigger()
{
    RFU_K7_REG_DECLARE_2(0x050b,0x060b);
    RFU_K7_OP_2(0x050b,0x060b);
    return 0;
}

int32_t sp1401::set_arb_bc_star_pos(uint32_t pos)
{
    RFU_K7_REG_DECLARE(0x0028);

    RFU_K7_R(0x0028);
    RFU_K7_REG(0x0028).addr = pos;
    RFU_K7_W(0x0028);
    return 0;
}

int32_t sp1401::set_arb_bc_samples(uint32_t samples)
{
    RFU_K7_REG_DECLARE(0x002f);

    RFU_K7_REG(0x002f).samples = samples;
    RFU_K7_W(0x002f);
    return 0;
}

uint32_t sp1401::get_arb_current_seg()
{
    RFU_K7_REG_DECLARE_2(0x050e,0x060e);
    RFU_K7_R_2(0x050e,0x060e);
    return RFU_K7_REG_2(0x050e,0x060e).seg;
}

uint32_t sp1401::get_arb_current_sample()
{
    RFU_K7_REG_DECLARE_2(0x0540,0x0640);
    RFU_K7_R_2(0x0540,0x0640);
    return RFU_K7_REG_2(0x0540,0x0640).sample;
}

uint32_t sp1401::get_arb_current_cycle()
{
    RFU_K7_REG_DECLARE_2(0x0541,0x0641);
    RFU_K7_R_2(0x0541,0x0641);
    return RFU_K7_REG_2(0x0541,0x0641).cycle;
}

double sp1401::get_arb_current_timer()
{
    RFU_K7_REG_DECLARE_2(0x0542,0x0642);
    RFU_K7_REG_DECLARE_2(0x0543,0x0643);
    RFU_K7_R_2(0x0542,0x0642);
    RFU_K7_R_2(0x0543,0x0643);
    return (RFU_K7_REG_2(0x0543,0x0643).time_l * 1.0 / 250e6 + RFU_K7_REG_2(0x0542,0x0642).time_h);
}

int32_t sp1401::arb_start()
{
    RFU_K7_REG_DECLARE_2(0x0509,0x0609);
    RFU_K7_REG_DECLARE(0x0549);
    RFU_K7_REG_2(0x0509,0x0609).abort	= 0;
    RFU_K7_REG_2(0x0509,0x0609).en		= 1;
    RFU_K7_REG_2(0x0509,0x0609).apc	= 0;
    RFU_K7_REG(0x0549).en = 1;
    RFU_K7_W_2(0x0509,0x0609);
    RFU_K7_W(0x0549);
    return 0;
}

int32_t sp1401::arb_stop()
{
    RFU_K7_REG_DECLARE_2(0x0509,0x0609);
    RFU_K7_REG_DECLARE(0x0549);
    RFU_K7_REG_2(0x0509,0x0609).abort	= 1;
    RFU_K7_REG_2(0x0509,0x0609).en		= 0;
    RFU_K7_REG(0x0549).en = 0;
    RFU_K7_W_2(0x0509,0x0609);
    RFU_K7_W(0x0549);
    return 0;
}

int32_t sp1401::w_eeprom(uint16_t addr,uint32_t length,const char *buf)
{
    RFU_K7_REG_DECLARE_2(0x161d,0x169d);
    RFU_K7_REG_DECLARE_2(0x161e,0x169e);
    RFU_K7_REG_DECLARE_2(0x161f,0x169f);
    RFU_K7_REG_2(0x161d,0x169d).mode = 0;	//write mode
    RFU_K7_W_2(0x161d,0x169d);
    sleep_ms(10);
    for (uint32_t i = 0;i < length * sizeof(char);i ++) {
        RFU_K7_REG_2(0x161e,0x169e).addr = addr + i;
        RFU_K7_W_2(0x161e,0x169e);
        sleep_ms(10);
        RFU_K7_REG_2(0x161f,0x169f).wr_data = unsigned(*(buf + i));
        RFU_K7_W_2(0x161f,0x169f);
        sleep_ms(10);
        RFU_K7_OP_2(0x161d,0x169d);
        sleep_ms(10);
    }
    return 0;
}

int32_t sp1401::r_eeprom(uint16_t addr,uint32_t length,char *buf)
{
    RFU_K7_REG_DECLARE_2(0x161d,0x169d);
    RFU_K7_REG_DECLARE_2(0x161e,0x169e);
    RFU_K7_REG_DECLARE_2(0x1620,0x16a0);
    RFU_K7_REG_2(0x161d,0x169d).mode = 1;	//read mode
    RFU_K7_W_2(0x161d,0x169d);
    for (uint32_t i = 0;i < length * sizeof(char);i ++) {
        RFU_K7_REG_2(0x161e,0x169e).addr = addr + i;
        RFU_K7_W_2(0x161e,0x169e);
        RFU_K7_OP_DELAY_2(0x161d,0x169d,10);
        RFU_K7_R_2(0x1620,0x16a0);
        buf[i] = RFU_K7_REG_2(0x1620,0x16a0).rd_data;
    }
    return 0;
}

int32_t sp1401::set_pwr_meas_src(pwr_meas_src_t src,bool posedge)
{
    RFU_K7_REG_DECLARE_2(0x1062,0x1092);
    RFU_K7_REG_2(0x1062,0x1092).iqcap_src_sel  = unsigned(src);
    RFU_K7_REG_2(0x1062,0x1092).iqcap_src_edge = 0 /*posedge ? 1 : 0*/ ;
    RFU_K7_W_2(0x1062,0x1092);
    return 0;
}

int32_t sp1401::set_pwr_meas_timeout(uint32_t us)
{
    double cycle = 1.0 / 62.5;
    RFU_K7_REG_DECLARE_2(0x1063,0x1093);
    RFU_K7_REG_2(0x1063,0x1093).iqcap_timeout = unsigned(round(us / cycle,0));
    RFU_K7_W_2(0x1063,0x1093);
    return 0;
}

int32_t sp1401::is_pwr_meas_timeout(bool &timeout)
{
    RFU_K7_REG_DECLARE_2(0x1063,0x1093);
    RFU_K7_R_2(0x1063,0x1093);
    timeout = (1 == RFU_K7_REG_2(0x1063,0x1093).iqcap_tmo) ? true : false;
    return 0;
}

int32_t sp1401::set_pwr_meas_trig_threshold(double if_pwr)
{
    RFU_K7_REG_DECLARE_2(0x1068,0x1098);
    RFU_K7_REG_2(0x1068,0x1098).threshold = unsigned(dBc_to_ad(g_0dBFS,-1 * if_pwr));
    RFU_K7_W_2(0x1068,0x1098);
    return 0;
}

int32_t sp1401::pwr_meas_start()
{
    RFU_K7_REG_DECLARE_2(0x1064,0x1094);
    RFU_K7_REG_DECLARE_2(0x1065,0x1095);

    RFU_K7OP_2(0x1065,0x1095,abort);

    RFU_K7_REG_2(0x1064,0x1094).op = 0;
    RFU_K7_W_2(0x1064,0x1094);

    RFU_K7_REG_2(0x1064,0x1094).op = 1;
    RFU_K7_W_2(0x1064,0x1094);
    return 0;
}

int32_t sp1401::pwr_meas_abort()
{
    RFU_K7_REG_DECLARE_2(0x1065,0x1095);
    RFU_K7_REG_2(0x1065,0x1095).abort = 1;
    RFU_K7_W_2(0x1065,0x1095);
    return 0;
}

int32_t sp1401::set_pwr_meas_samples(uint32_t samples)
{
    RFU_K7_REG_DECLARE_2(0x1066,0x1096);
    RFU_K7_REG_2(0x1066,0x1096).samples = samples;
    RFU_K7_W_2(0x1066,0x1096);
    return 0;
}

int32_t sp1401::get_pwr_meas_samples(uint32_t &samples)
{
    RFU_K7_REG_DECLARE_2(0x1066,0x1096);
    RFU_K7_R_2(0x1066,0x1096);
    samples = RFU_K7_REG_DATA_2(0x1066,0x1096);
    return 0;
}

int32_t sp1401::get_pwr_meas_state(pwr_meas_state_t &state)
{
    RFU_K7_REG_DECLARE_2(0x1074,0x10a4);
    RFU_K7_R_2(0x1074,0x10a4);
    state = pwr_meas_state_t(RFU_K7_REG_2(0x1074,0x10a4).state);
    return 0;
}

int32_t sp1401::get_pwr_meas_peak(double &pwr)
{
    RFU_K7_REG_DECLARE_2(0x1075,0x10a5);
    RFU_K7_R_2(0x1075,0x10a5);
    ad_to_dBc(g_0dBFS,RFU_K7_REG_2(0x1075,0x10a5).iq_sum,pwr);
    return 0;
}

int32_t sp1401::get_pwr_meas_pwr(double &pwr)
{
    RFU_K7_REG_DECLARE_2(0x1076,0x10a6);
    RFU_K7_REG_DECLARE_2(0x1077,0x10a7);
    RFU_K7_R_2(0x1076,0x10a6);
    RFU_K7_R_2(0x1077,0x10a7);
    int64_t iq_sum_l = int64_t(RFU_K7_REG_2(0x1077,0x10a7).iq_sum_l);
    int64_t iq_sum_h = int64_t(RFU_K7_REG_2(0x1076,0x10a6).iq_sum_h);
    uint32_t samples = 0;
    get_pwr_meas_samples(samples);
    double iq_sum = (double(iq_sum_l + (iq_sum_h << 32)) / double(samples));
    ad_to_dBc(g_0dBFS,iq_sum,pwr);
    return 0;
}

int32_t sp1401::get_ads5474(int64_t &ad)
{
    RFU_K7_REG_DECLARE_2(0x00b9,0x20b9);
    RFU_K7_REG_DECLARE_2(0x00e5,0x20e5);

    RFU_K7_OP_2(0x00e5,0x20e5);
    RFU_K7_WAIT_IDLE_2(0x00e5,0x20e5,1,10000);
    RFU_K7_R_2(0x00b9,0x20b9);

    RFU_K7_OP_2(0x00e5,0x20e5);
    RFU_K7_WAIT_IDLE_2(0x00e5,0x20e5,1,10000);
    RFU_K7_R_2(0x00b9,0x20b9);

    ad = (RFU_K7_REG_2(0x00b9,0x20b9).iq_avg) << 2;
    return 0;
}

int32_t sp1401::get_ads5474_manual(int64_t &ad)
{
//     ad = 0;
//     int16_t I[4096] = {0};
//     int16_t Q[4096] = {0};
//     double sum_I = 0.0;
//     double sum_Q = 0.0;
// 
//     INT_CHECK(set_iq_cap_samples(4096));
//     INT_CHECK(iq_cap());
//     INT_CHECK(DDR.iq2buf(m_cal_file->get_rf_idx(),I,Q,4096));
// 
//     for (int32_t i = 0;i < 4096;i ++) {
//         sum_I += pow((double)I[i],2);
//         sum_Q += pow((double)Q[i],2);
//     }
//     ad = (int64_t)((sum_I + sum_Q) / 4096);
    return 0;
}

int32_t sp1401::set_rx_if_filter_sw(rx_if_filter_t filter)
{
    RFU_K7_REG_DECLARE_2(0x1668,0x16e8);
    RFU_K7_REG_2(0x1668,0x16e8).fil_sw = (_100MHz == filter ? 1 : 0);
    RFU_K7_W_2(0x1668,0x16e8);
    return 0;
}

int32_t sp1401::get_ad7680(uint32_t &det)
{
    RFU_K7_REG_DECLARE_2(0x1611,0x1691);
    RFU_K7_REG_DECLARE_2(0x1612,0x1692);
    RFU_K7_WAIT_IDLE_2(0x1611,0x1691,0,1000);
    RFU_K7_OP_2(0x1611,0x1691);
    RFU_K7_WAIT_IDLE_2(0x1611,0x1691,0,1000);

    RFU_K7_R_2(0x1612,0x1692);
    det = RFU_K7_REG_2(0x1612,0x1692).adc;
    return 0;
}

#define IMPL_FUNC_FTP_PUT(report) \
int32_t sp1401::ftp_put##report() \
{   ftp ftp_worker("10.10.0.10","",""); \
    path p_host(report->full_path()); \
    path p_remote("rf-driver\\database\\SP9500"); \
    string file_name_remote(report->get_header().sn); \
    file_name_remote += "_"; \
    file_name_remote += p_host.filename().string(); \
    for (uint32_t i = 0;i < 3;i ++) { \
        if (ftp_worker.put(p_host.string(),p_remote.string(),file_name_remote)) { \
            return 0; \
        } else { \
            if (sp1401::_ftp_retry_call_back.empty()) { return -1; } \
            if (sp1401::_ftp_retry_call_back()) continue; \
            else return -1; \
        } \
    } \
    return -1; \
}

IMPL_FUNC_FTP_PUT(_tr_rf_tx_freq_res_test)
IMPL_FUNC_FTP_PUT(_tr_if_tx_freq_res_test)
IMPL_FUNC_FTP_PUT(_tr_rf_rx_freq_res_test)
IMPL_FUNC_FTP_PUT(_tr_if_rx_freq_res_test)
IMPL_FUNC_FTP_PUT(_tr_tx_phase_noise_test)
IMPL_FUNC_FTP_PUT(_tr_tx_noise_floor_test)
IMPL_FUNC_FTP_PUT(_tr_tx_lo_ld_test)
IMPL_FUNC_FTP_PUT(_tr_tx_pwr_mod_sw_test)
IMPL_FUNC_FTP_PUT(_tr_tx_filter_sw_test)
IMPL_FUNC_FTP_PUT(_tr_tx_io_sw_test)

IMPL_FUNC_FTP_PUT(_cr_tx_passband_freq_res_160_cal)
IMPL_FUNC_FTP_PUT(_cr_rx_passband_freq_res_160_cal)
IMPL_FUNC_FTP_PUT(_cr_tx_base_pwr_cal)
IMPL_FUNC_FTP_PUT(_cr_tx_pwr_op_cal)
IMPL_FUNC_FTP_PUT(_cr_tx_pwr_io_cal)
IMPL_FUNC_FTP_PUT(_cr_rx_ref_cal)
IMPL_FUNC_FTP_PUT(_cr_rx_pwr_op_cal)
IMPL_FUNC_FTP_PUT(_cr_rx_pwr_io_cal)
