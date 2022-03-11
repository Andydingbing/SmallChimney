#include "sp3301.h"
#include "../chip/reg_def_sp9500.h"
#include "algo_fit.hpp"
#include "gen_ini_file.hpp"
#include "sleep_common.h"
#include "self_cal_helper.h"
#include <string>

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;
using namespace ns_sp1401;
using namespace ns_sp2401;

double sp3301::temp_cal_star = 25.0;
double sp3301::temp_cal_stop = 60.0;

double sp3301::tx_tc_coef[12][6] = {
    {-13.7312134571261186,-0.0629276521712293,+0.0025176156861272,-0.0000732821028996,+0.0000009380902693,-0.0000000046087530},
    {-14.0220728659690064,-0.0688810354240079,+0.0022810665211714,-0.0000575032396896,+0.0000006403458238,-0.0000000027666212},
    {-14.2008383209365636,-0.0717124346721868,+0.0021607321332127,-0.0000519136496466,+0.0000005427827502,-0.0000000021809891},
    {-15.2698164299725931,+0.0213073777175080,-0.0023948793684254,+0.0000561645850000,-0.0000007016972166,+0.0000000034032491},
    {-15.4906872635086934,+0.0197045719251947,-0.0024012055180567,+0.0000579407374724,-0.0000007399111859,+0.0000000036624557},
    {-16.1030108435888479,+0.0841407154923605,-0.0060452488568785,+0.0001575170994113,-0.0000020610304997,+0.0000000104525773},
    {-10.9669063203231030,-0.5979639966743983,+0.0281888397529856,-0.0006820665566378,+0.0000079552669652,-0.0000000361715028},
    {-16.1458867522741798,+0.0709512979661163,-0.0056331887766860,+0.0001358742225996,-0.0000016608693874,+0.0000000078753890},
    {-42.4135058079832987,+3.3539141668022214,-0.1681438454172170,+0.0041003856864419,-0.0000493318802131,+0.0000002336750072},
    {-14.8270244354544172,-0.1051484809315957,+0.0032885589536445,-0.0001019095007723,+0.0000014517600635,-0.0000000081642260},
    {-16.9899493065312583,+0.2483518608654298,-0.0165707363688776,+0.0004222477705433,-0.0000053426791656,+0.0000000263145082},
    {-12.5075039654556850,-0.3279871332373878,+0.0122981246260907,-0.0002957749056152,+0.0000033569128555,-0.0000000148411204},
};

double sp3301::rx_tc_coef[12][6] = {
    {-12.7461916565088345,-0.5675610706895694,+0.0298820216802675,-0.0007378646376766,+0.0000087108621175,-0.0000000400584204},
    {-12.7461916565088345,-0.5675610706895694,+0.0298820216802675,-0.0007378646376766,+0.0000087108621175,-0.0000000400584204},
    {-22.4213075425261934,+0.6884183297956783,-0.0341971386825836,+0.0008473324245523,-0.0000103902368687,+0.0000000498820317},
    {-20.3386553144923603,+0.4322378677894460,-0.0214713505958134,+0.0005357523678792,-0.0000066879808571,+0.0000000327917125},
    {-20.8279534718378727,+0.4707362036678293,-0.0230994830248306,+0.0005776449818016,-0.0000072522930554,+0.0000000358131207},
    {-20.8379308377108430,+0.4913308792739430,-0.0247806231336308,+0.0006264826142860,-0.0000079128288627,+0.0000000392622589},
    {-15.5214883740408194,-0.1820627362990533,+0.0085130680368894,-0.0001725121389396,+0.0000014093494641,-0.0000000031426327},
    {-20.2636157637376400,+0.3904951482328652,-0.0194043252343287,+0.0004869072845675,-0.0000061424883110,+0.0000000304720604},
    {-20.6465321977551781,+0.4762107405366010,-0.0244775316563817,+0.0006249561868787,-0.0000079667395634,+0.0000000397388244},
    {-21.2300245236968728,+0.5806368286864927,-0.0301760563394241,+0.0007649678114279,-0.0000096345726096,+0.0000000474685055},
    {-21.4319091374456399,+0.6345328756198703,-0.0333616311273781,+0.0008524910357970,-0.0000107898547378,+0.0000000532902994},
    {-20.4481420074096860,+0.5204172441628423,-0.0285343069279326,+0.0007448393639637,-0.0000096415820063,+0.0000000485445166},
};

#define DECL_DYNAMIC_SP1401 sp1401     *sp1401 = _sp1401[index];
#define DECL_DYNAMIC_SP2401 sp2401_r1a *sp2401 = _sp2401_r1a[index];

#define SP1401_R1A dynamic_cast<sp1401_r1a *>(sp1401)
#define SP1401_R1C dynamic_cast<sp1401_r1c *>(sp1401)
#define SP1401_R1F dynamic_cast<sp1401_r1f *>(sp1401)

#define DECL_RF_VER hw_ver_t RF_VER = hw_ver_t(sp1401->hw_ver());

#define DECL_DYNAMIC_DDR dma_mgr *ddr = _dma_mgr[K7_IDX(index)];

#define CAL_FILE_R1A dynamic_cast<cal_file_r1ab *>(sp1401->cf())
#define CAL_FILE_R1C dynamic_cast<cal_file_r1cd *>(sp1401->cf())

sp3301::active_t::active_t()
{
    k7_0 = false;
    k7_1 = false;
    s6   = false;

    for (uint32_t i = 0;i < ARRAY_SIZE(sp1401);++i) {
        sp1401[i] = false;
    }
}

sp3301::sp3301(uint32_t rfu_idx):
    m_k7_0(ass_k7_name(0,rfu_idx).c_str()),
    m_k7_1(ass_k7_name(1,rfu_idx).c_str()),
    m_s6(ass_s6_name(rfu_idx).c_str()),
    m_rfu_idx(rfu_idx),
    m_is_program_k7_0(true),
    m_is_program_k7_1(true)
{
    for (uint32_t i = 0;i < g_max_rf;i ++) {
        _sp1401_r1a[i] = new sp1401_r1a(i,rfu_idx);
        _sp1401_r1c[i] = new sp1401_r1c(i,rfu_idx);
        _sp1401_r1e[i] = new sp1401_r1e(i,rfu_idx);
        _sp1401_r1f[i] = new sp1401_r1f(i,rfu_idx);
        _sp2401_r1a[i] = new sp2401_r1a(i);
        _sp1401[i] = _sp1401_r1a[i];

        _I[i] = nullptr;
        _Q[i] = nullptr;
    }

    for (uint32_t i = 0;i < g_max_rf/2;i ++) {
        _dma_mgr[i] = nullptr;
    }

    for (int32_t i = 0;i < g_max_rf;i ++) {
        m_io_mode[i] = io_mode_t::OUTPUT;
        m_tx_freq[i] = 2400000000;
        m_tx_pwr[i] = -20.0;
        m_rx_freq[i] = 2400000000;
        m_ref[i] = -20.0;
    }
}

sp3301::~sp3301()
{
}

sp3301 &sp3301::instance(uint32_t rfu_idx)
{
    switch (rfu_idx) {
        case 0 : {static sp3301 sp3301_0(0);return sp3301_0;}
        case 1 : {static sp3301 sp3301_1(1);return sp3301_1;}
        case 2 : {static sp3301 sp3301_2(2);return sp3301_2;}
        case 3 : {static sp3301 sp3301_3(3);return sp3301_3;}
        case 4 : {static sp3301 sp3301_4(4);return sp3301_4;}
        default: {static sp3301 reserve(UINT32_MAX);return reserve;}
    }
}

uint32_t sp3301::channels()
{
    return g_max_rf;
}

int32_t sp3301::set_sn(const uint32_t index,const char *sn)
{
    DECL_DYNAMIC_SP1401
//    sp1401->set_sn_major(sn);
    return 0;
}

int32_t sp3301::get_sn(const uint32_t index,char *sn)
{
    DECL_DYNAMIC_SP1401
    sp1401->get_sn_major(sn);
    return 0;
}

const item_table_base* sp3301::data_base(const uint32_t index,const int32_t kase) const
{
    DECL_DYNAMIC_SP1401

    return sp1401->data_base()->db(kase);
}

int32_t sp3301::data_base_add(const uint32_t index,const int32_t kase,void *data)
{
    DECL_DYNAMIC_SP1401

    return sp1401->data_base()->add(kase,data);
}

int32_t sp3301::prepare_kase(const uint32_t index,const int32_t kase,const std::string freq_str,const bool is_exp)
{
    DECL_DYNAMIC_SP1401

    set_helper::range_freq<uint64_t> freqs;
    set_helper::parse(freq_str,freqs);

    return sp1401->data_base()->prepare(cal_table_t::_from_integral(kase),freqs.freq,is_exp);
}

//sp3301::rfu_info_t::rfu_info_t(const char *k7_0,const char *k7_1,const char *s6,const char *sn):
//    k7_0_ver(0),
//    k7_1_ver(0),
//    s6_ver(0)
//{
//    strcpy(this->rsrc_name.k7_0,k7_0);
//    strcpy(this->rsrc_name.k7_1,k7_1);
//    strcpy(this->rsrc_name.s6,s6);
//    strcpy(this->sn,sn);
//}

sp3301::active_t sp3301::is_actived()
{
    return m_active;
}

int32_t sp3301::get_ocxo(uint16_t &value)
{
    uint32_t index = 0;
    DECL_DYNAMIC_SP1401;
    data_m_x9119 data;

    ((x9119_table_t *)(sp1401->data_base()->db(cal_table_t::X9119)))->get(FREQ_M(10),data);
    value = data.value;
    return 0;
}

int32_t sp3301::init()
{
    vector<string> str_rsrc_rfu;
    vector<string>::iterator iter_rsrc_rfu;
    pci_dev_vi pci_dev;

    char str_product_form[64];
    char str_rsrc_section[64];
    char str_dev_key[64];
    memset(str_product_form,0,sizeof(str_product_form));
    memset(str_rsrc_section,0,sizeof(str_rsrc_section));
    memset(str_dev_key,0,sizeof(str_dev_key));

    gen_ini_file ini_file(CONFIG_FILE_PATH);

    ini_file.get_config_str_value("Product Form","Form",str_product_form);
    strcpy(str_rsrc_section,"RESOURCE");
    strcat(str_rsrc_section,str_product_form);

    sprintf(str_dev_key,"RFU%u_K7_0",m_rfu_idx);
    ini_file.get_config_str_value(str_rsrc_section,str_dev_key,m_rfu_info.k7_0);
    sprintf(str_dev_key,"RFU%u_K7_1",m_rfu_idx);
    ini_file.get_config_str_value(str_rsrc_section,str_dev_key,m_rfu_info.k7_1);
    sprintf(str_dev_key,"RFU%u_S6",m_rfu_idx);
    ini_file.get_config_str_value(str_rsrc_section,str_dev_key,m_rfu_info.s6);

    pci_dev.get_devs(str_rsrc_rfu);

    for (iter_rsrc_rfu = str_rsrc_rfu.begin();iter_rsrc_rfu != str_rsrc_rfu.end();iter_rsrc_rfu ++ ) {
        if (!strcmp(m_rfu_info.k7_0,iter_rsrc_rfu->c_str())) {
            m_active.k7_0 = true;
            INT_CHECK(m_k7_0.init(m_rfu_info.k7_0,0x10ee,0x0007));
        }
        if (!strcmp(m_rfu_info.k7_1,iter_rsrc_rfu->c_str())) {
            m_active.k7_1 = true;
            INT_CHECK(m_k7_1.init(m_rfu_info.k7_1,0x10ee,0x0007));
        }
        if (!strcmp(m_rfu_info.s6,iter_rsrc_rfu->c_str())) {
            m_active.s6 = true;
            INT_CHECK(m_s6.init(m_rfu_info.s6,0x10ee,0x2411));
        }
    }

    if (m_active.k7_0 == false && m_active.k7_1 == false && m_active.s6 == false) {
        Log.set_last_err("missing rfu%d",m_rfu_idx);
        return -1;
    }

    if (m_active.s6) {
        if (m_active.k7_0) {
            INT_CHECK(_sp2401_r1a[2]->open_board(&m_k7_0,&m_s6));
            INT_CHECK(_sp2401_r1a[3]->open_board(&m_k7_0,&m_s6));
            INT_CHECK(_sp2401_r1a[2]->get_s6_ver(m_rfu_info.s6_ver));

            _sp1401_r1a[2]->connect(&m_k7_0);
            _sp1401_r1a[3]->connect(&m_k7_0);
            _sp1401_r1c[2]->connect(&m_k7_0);
            _sp1401_r1c[3]->connect(&m_k7_0);
            _sp1401_r1e[2]->connect(&m_k7_0);
            _sp1401_r1e[3]->connect(&m_k7_0);
            _sp1401_r1f[2]->connect(&m_k7_0);
            _sp1401_r1f[3]->connect(&m_k7_0);

            instance_sp1401(2);
            instance_sp1401(3);

            _sp1401[2]->set_tx_en_tc(true);
            _sp1401[2]->set_rx_en_tc(true);
            _sp1401[3]->set_tx_en_tc(true);
            _sp1401[3]->set_rx_en_tc(true);

            _dma_mgr[0] = new dma_mgr(&m_k7_0);
            INT_CHECK(_dma_mgr[0]->init());
            INT_CHECK(_sp1401[2]->get_k7_ver(m_rfu_info.k7_0_ver));
        }
        if (m_active.k7_1) {
            INT_CHECK(_sp2401_r1a[0]->open_board(&m_k7_1,&m_s6));
            INT_CHECK(_sp2401_r1a[1]->open_board(&m_k7_1,&m_s6));
            INT_CHECK(_sp2401_r1a[0]->get_s6_ver(m_rfu_info.s6_ver));

            _sp1401_r1a[0]->connect(&m_k7_1);
            _sp1401_r1a[1]->connect(&m_k7_1);
            _sp1401_r1c[0]->connect(&m_k7_1);
            _sp1401_r1c[1]->connect(&m_k7_1);
            _sp1401_r1e[0]->connect(&m_k7_1);
            _sp1401_r1e[1]->connect(&m_k7_1);
            _sp1401_r1f[0]->connect(&m_k7_1);
            _sp1401_r1f[1]->connect(&m_k7_1);

            instance_sp1401(0);
            instance_sp1401(1);

            _sp1401[0]->set_tx_en_tc(true);
            _sp1401[0]->set_rx_en_tc(true);
            _sp1401[1]->set_tx_en_tc(true);
            _sp1401[1]->set_rx_en_tc(true);

            _dma_mgr[1] = new dma_mgr(&m_k7_1);
            INT_CHECK(_dma_mgr[1]->init());
            INT_CHECK(_sp1401[0]->get_k7_ver(m_rfu_info.k7_1_ver));
        }
    }

    sprintf(m_rfu_info.sn,"SP2401R1BRFUD160%08x%08x",m_rfu_info.k7_0_ver,m_rfu_info.k7_1_ver);
    return 0;
}

int32_t sp3301::set_program_bit(char *path,bool k7_0,bool k7_1)
{
    m_is_program_k7_0 = k7_0;
    m_is_program_k7_1 = k7_1;
    if (path) {
        strcpy(m_bit_path,path);
    }
    return 0;
}

int32_t sp3301::program_bit()
{
    if ((false == m_is_program_k7_0) && (false == m_is_program_k7_1)) {
        return 0;
    }

    if (!m_active.s6) {
        return -1;
    }

    char str_child_dir = 'A' - 1;
    char bit_path[128] = {0};
    bool is_conflict[2] = {false,false};

    if (m_is_program_k7_0) {
        str_child_dir = rf_ver2child_dir(get_rf_ver(2),get_rf_ver(3));
        if (str_child_dir >= 'A') {
            strcpy(bit_path,m_bit_path);
            strcat(bit_path,"\\");
            bit_path[strlen(bit_path)] = str_child_dir;
            strcat(bit_path,"\\");
            strcat(bit_path,"p0_top.bit");
            Log.stdprintf("k7_0 bit path = %s\n",bit_path);
            INT_CHECK(_sp2401_r1a[2]->set_fpga_bit(bit_path));
        } else {
            is_conflict[0] = true;
            Log.set_last_err("conflicted version of rf2(%s) & rf3(%s)",
                             sp1401::hw_ver2str(get_rf_ver(2)).c_str(),
                             sp1401::hw_ver2str(get_rf_ver(3)).c_str());
        }
    }

    ZERO_ARRAY(bit_path);
    if (m_is_program_k7_1) {
        str_child_dir = rf_ver2child_dir(get_rf_ver(0),get_rf_ver(1));
        if (str_child_dir >= 'A') {
            strcpy(bit_path,m_bit_path);
            strcat(bit_path,"\\");
            bit_path[strlen(bit_path)] = str_child_dir;
            strcat(bit_path,"\\");
            strcat(bit_path,"p1_top.bit");
            Log.stdprintf("k7_1 bit path = %s\n",bit_path);
            INT_CHECK(_sp2401_r1a[0]->set_fpga_bit(bit_path));
        }
        else {
            is_conflict[1] = true;
            Log.set_last_err("conflicted version of rf0(%s) & rf1(%s)",
                             sp1401::hw_ver2str(get_rf_ver(0)).c_str(),
                             sp1401::hw_ver2str(get_rf_ver(1)).c_str());
        }
    }

    return (is_conflict[0] || is_conflict[1]) ? -1 : 0;
}

int32_t sp3301::get_rf_port()
{
    int32_t no = 0;
    for (int i = 0;i < 4;i ++) {
        if (m_active.sp1401[i] == true) {
            no ++;
        }
    }
    return no;
}

hw_ver_t sp3301::get_rf_ver(uint32_t index)
{
    return hw_ver_t(_sp1401[index]->hw_ver());
}

int32_t sp3301::get_sn(char *sn)
{
    strcpy(sn,m_rfu_info.sn);
    return 0;
}

int32_t sp3301::get_ver(char *ver)
{
    strcpy(ver,"SP2401R1BRFUD160");
    return 0;
}

string sp3301::ass_k7_name(uint32_t k7_idx,uint32_t rfu_idx)
{
    char name[16] = {0};
    memset(name,0,sizeof(name));
    sprintf(name,"rfu%u_k7_%u",rfu_idx,k7_idx);
    return string(name);
}

string sp3301::ass_s6_name(uint32_t rfu_idx)
{
    char name[16] = {0};
    memset(name,0,sizeof(name));
    sprintf(name,"rfu%u_s6",rfu_idx);
    return string(name);
}

char sp3301::rf_ver2child_dir(hw_ver_t ver0,hw_ver_t ver1)
{
    /*
     * ver0        ver1        decision
     * HW_ERROR    HW_ERROR    HW_VER_SP9500_MAX-1
     * HW_ERROR    X           X
     * X           HW_ERROR    X
     * X           X           X
     * X           Y           HW_ERROR
     *
     */

    hw_ver_t decision_ver = HW_ERROR;
    if (HW_ERROR == ver0)
        decision_ver = ((HW_ERROR == ver1) ? hw_ver_t(HW_VER_SP9500_MAX - 1) : ver1);
    else
        decision_ver = (((ver0 != ver1) && (ver1 != HW_ERROR)) ? HW_ERROR : ver0);

    switch (decision_ver) {
        case R1A : return 'A';
        case R1B : return 'B';
        case R1C : return 'C';
        case R1D : return 'D';
        case R1E : return 'E';
        case R1F : return 'F';
        case HW_ERROR :
        case HW_VER_SP9500_MAX : return 'A' - 1;
    }
    return 'A' - 1;
}

int32_t sp3301::set_tx_en_tc(const uint32_t index,const bool en)
{
    DECL_DYNAMIC_SP1401
    sp1401->set_tx_en_tc(en);
    return 0;
}

int32_t sp3301::set_tx_state(uint32_t index,bool state)
{
    DECL_DYNAMIC_SP1401
    sp1401->set_tx_modulator_en(state);
    return 0;
}

int32_t sp3301::set_tx_pwr(uint32_t index,float pwr)
{
    DECL_DYNAMIC_SP1401
    DECL_DYNAMIC_SP2401
    DECL_RF_VER

    uint64_t freq = m_tx_freq[index];
    io_mode_t mode = io_mode_t::_from_integral(m_io_mode[index]);

    switch (RF_VER) {
        case R1A : case R1B : {
            double d_gain = 0.0,att_offset = 0.0;
            int32_t att1 = 0,att2 = 0,att3 = 0;
            r1a::tx_pa_att_t pa_att = r1a::TX_ATT;

//            CAL_FILE_R1A->get_tx_pwr(freq,double(pwr),mode,d_gain,att1,att2,pa_att,att3);
//            CAL_FILE_R1A->get_tx_att(freq,mode,att1 + att2 + att3,att_offset);

            INT_CHECK(((sp1401_r1a *)sp1401)->set_tx_pa_att_sw(pa_att));
            INT_CHECK(((sp1401_r1a *)sp1401)->sync_set_tx_gain(att1,att2,att3,d_gain + att_offset));
            break;
        }
        case R1C : case R1D : case R1E : case R1F : {
            double att0 = 0.0;
            double att1 = 0.0;
            double att2 = 0.0;
            double att3 = 0.0;
            double d_gain = 0.0;
            const bool en_tc = sp1401->is_tx_en_tc();
            double temp_call = 0.0;
            double temp_curr = 0.0;
            double *coef = nullptr;

//            if (io_mode_t::OUTPUT == mode._to_integral()) {
//                tx_pwr_op_table_r1c::data_m_t data;
//                CAL_FILE_R1C->m_tx_pwr_op->get(RF_VER,freq,double(pwr),&data);
//                att0 = data.att0 / 2.0;
//                att1 = data.att1 / 2.0;
//                att2 = data.att2 / 2.0;
//                att3 = data.att3 / 2.0;
//                d_gain = double(data.d_gain + CAL_FILE_R1C->m_tx_att_op->get(freq,double(pwr)));
//                temp_call = en_tc ? data.temp_5 : 0.0;
//            } else if (io_mode_t::IO == mode._to_integral()) {
//                tx_pwr_io_table_r1c::data_m_t data;
//                CAL_FILE_R1C->m_tx_pwr_io->get(RF_VER,freq,double(pwr),&data);
//                att0 = data.att0 / 2.0;
//                att1 = data.att1 / 2.0;
//                att2 = data.att2 / 2.0;
//                att3 = data.att3 / 2.0;
//                d_gain = double(data.d_gain + CAL_FILE_R1C->m_tx_att_io->get(freq,double(pwr)));
//                temp_call = en_tc ? data.temp_5 : 0.0;
//            }

            if (en_tc) {
                if (((sp1401_r1c *)sp1401)->get_tx_avg_temp(temp_curr) == true) {
                    temp_curr = temp_curr >= temp_cal_star ? temp_curr : temp_cal_star;
                    temp_curr = temp_curr <= temp_cal_stop ? temp_curr : temp_cal_stop;

                    coef = tx_tc_coef[SERIE_INDEX(linear_quantify<uint64_t>(tx_freq_star,FREQ_M(500),freq),tx_freq_star,FREQ_M(500))];
                    d_gain += polynomial<double>(coef,5,temp_call) - polynomial<double>(coef,5,temp_curr);
                }
            }

            INT_CHECK(((sp1401_r1c *)sp1401)->set_tx_att(att0,att1,att2,att3));
            INT_CHECK(sp2401->set_tx_pwr_comp(d_gain));
            break;
        }
        default:break;
    }
    m_tx_pwr[index] = double(pwr);
    return 0;
}

int32_t sp3301::set_tx_freq(uint32_t index,uint64_t freq)
{
    DECL_DYNAMIC_SP1401
    DECL_DYNAMIC_SP2401

    uint64_t freq_rf = freq / int64_t(g_rf_freq_space) * int64_t(g_rf_freq_space);
    double freq_duc = double(freq - freq_rf);

//    switch (sp1401->get_hw_ver()) {
//        case R1A : case R1B : {
//            double th = 0.0;
//            uint16_t am_i = 0,am_q = 0;
//            int16_t dc_i = 0,dc_q = 0;

//            CAL_FILE_R1A->m_tx_sb->get(freq_rf,th,am_i,am_q);
//            CAL_FILE_R1A->m_tx_lol->get(freq_rf,dc_i,dc_q);
//            INT_CHECK(sp2401->set_tx_phase_rotate_I(th));
//            INT_CHECK(sp2401->set_tx_amplitude_balance(am_i,am_q));
//            INT_CHECK(sp2401->set_tx_dc_offset(dc_i,dc_q));
//            break;
//        }
//        case R1C : case R1D : case R1E : case R1F : {
//            tx_lol_table_r1cd::data_m_t data_lol;
//            tx_sb_table_r1cd::data_m_t data_sb;

//            CAL_FILE_R1C->m_tx_lol->get(freq_rf,&data_lol);
//            CAL_FILE_R1C->m_tx_sb->get(freq_rf,&data_sb);
//            INT_CHECK(sp2401->set_tx_dc_offset(uint16_t(data_lol.dc_i),uint16_t(data_lol.dc_q)));
//            INT_CHECK(sp2401->set_tx_phase_rotate_I(double(data_sb.th)));
//            INT_CHECK(sp2401->set_tx_amplitude_balance(data_sb.am_i,data_sb.am_q));
//            break;
//        }
//        default:break;
//    }
    INT_CHECK(sp1401->set_tx_freq(freq_rf));
    INT_CHECK(sp2401->set_duc_dds(freq_duc));
    m_tx_freq[index] = freq;
    INT_CHECK(set_tx_pwr(index,float(m_tx_pwr[index])));
    INT_CHECK(set_tx_bw(index,sp1401->bw()));
    return 0;
}

int32_t sp3301::get_tx_freq(uint32_t index,uint64_t &freq)
{
    freq = m_tx_freq[index];
    return 0;
}

int32_t sp3301::set_tx_bw(uint32_t index,bw_t bw)
{
    DECL_DYNAMIC_SP1401
    DECL_DYNAMIC_SP2401

    return 0;

//    switch (sp1401->get_hw_ver()) {
//        case R1C : case R1D : case R1E : case R1F : {
//            double real[dl_filter_tap_2i] = {0.0};
//            double imag[dl_filter_tap_2i] = {0.0};

//            CAL_FILE_R1C->set_bw(bw);
//            if (_80M == bw) {
//                tx_filter_80m_table::data_m_t data;
//                CAL_FILE_R1C->m_tx_filter_80m->get(m_tx_freq[index],&data);
//                data._2double(real,imag);
//                sp2401->set_tx_filter(real,imag);
//            } else if (_160M == bw) {
//                tx_filter_160m_table::data_m_t data;
//                CAL_FILE_R1C->m_tx_filter_160m->get(m_tx_freq[index],&data);
//                data._2double(real,imag);
//                sp2401->set_tx_filter(real,imag);
//            }
//            return 0;
//        }
//        default:return 0;
//    }
}

int32_t sp3301::set_tx_src(uint32_t index,sp2401_r1a::da_src_t src)
{
    DECL_DYNAMIC_SP2401
    INT_CHECK(sp2401->set_dds_src(src));
    return 0;
}

int32_t sp3301::set_src_freq(uint32_t index,uint64_t freq)
{
    DECL_DYNAMIC_SP2401
    INT_CHECK(sp2401->set_dds1((double)freq));
    return 0;
}

int32_t sp3301::arb_load(uint32_t index, const char *path)
{
    DECL_DYNAMIC_DDR
    uint32_t index_2 = brother_idx(index);

    sp1401 *sp1401[2] = {_sp1401[index],_sp1401[index_2]};
    FILE *fp[2] = {nullptr,nullptr};
    pci_dev_vi *_k7 = sp1401[0]->k7();

    uint32_t spls_total = 0;
    uint32_t spls_left = 0;
    uint32_t spls_trsed = 0;
    uint32_t spls_trsing = 0;
    uint32_t ddr_addr = 0;
    uint32_t cnt = 0;
    unsigned inter = 1;

    ns_arb::param_t arb_param[2];
    list<ns_arb::seg_header_t>::iterator iter_seg_header;

    INT_CHECK(sp1401[0]->arb()->load(path));
    if (false == sp1401[1]->arb()->is_loaded()) {
        INT_CHECK(sp1401[1]->arb()->load(path));
    }

    if ((fp[0] = sp1401[0]->arb()->load(&arb_param[0],nullptr)) == nullptr) {
        return -1;
    }
    if ((fp[1] = sp1401[1]->arb()->load(&arb_param[1],nullptr)) == nullptr) {
        return -1;
    }

    if (arb_param[0].samples != arb_param[1].samples) {
        Log.set_last_err("samples file0:%d != file1:%d",arb_param[0].samples,arb_param[1].samples);
        return -1;
    }

    iter_seg_header = arb_param[0].seg_header.begin();

    INT_CHECK(sp1401[0]->arb_stop());
    INT_CHECK(sp1401[1]->arb_stop());
    INT_CHECK(sp1401[0]->set_arb_segments(uint16_t(arb_param[0].header.segments)));
    INT_CHECK(sp1401[1]->set_arb_segments(uint16_t(arb_param[1].header.segments)));

    RFU_K7_REG_DECLARE(0x0544);
    for (iter_seg_header = arb_param[0].seg_header.begin();iter_seg_header != arb_param[0].seg_header.end();iter_seg_header ++) {
        inter = unsigned(245760000.0f / iter_seg_header->sr);
        RFU_K7_REG(0x0544).inter = inter;
        RFU_K7_W(0x0544);
    }

    RFU_K7_REG_DECLARE(0x0028);
    RFU_K7_REG_DECLARE(0x002f);
    RFU_K7_REG(0x0028).flag = 0;

    spls_total = arb_param[0].samples + arb_param[1].samples;
    spls_left = spls_total;
    Log.stdprintf("prepare fpga dma read,total samples(*2ed):%d\n",spls_total);
    while(spls_left > 0)
    {
        cnt ++;
        spls_trsing = spls_left > T_BLOCK_SAMPLES ? T_BLOCK_SAMPLES : spls_left;

        ddr_addr = spls_trsed >> 1;
        RFU_K7_REG(0x002f).samples = spls_trsing;
        RFU_K7_W(0x002f);
        RFU_K7_REG(0x0028).addr = ddr_addr;
        RFU_K7_W(0x0028);

        for (uint32_t i = 0;i < spls_trsing / 2;i ++) {
            fread(ddr->usr_spc() + i * 2,    sizeof(iq_data_t),1,fp[1]);
            fread(ddr->usr_spc() + i * 2 + 1,sizeof(iq_data_t),1,fp[0]);
        }

        INT_CHECK(ddr->fpga_r(spls_trsing));

        spls_trsed += spls_trsing;
        spls_left = spls_total - spls_trsed;
        Log.stdprintf("dma%-4d done,samples(*2ed):%-8d total(*2ed):%-10d\r",cnt,spls_trsing,spls_trsed);
    }
    sp1401[0]->set_arb_bc_star_pos(0);
    sp1401[0]->set_arb_bc_samples(spls_total);

    Log.stdprintf("\n");
    Log.stdprintf("all done\n");
    return 0;
}

int32_t sp3301::set_arb_en(uint32_t index,bool en)
{
    DECL_DYNAMIC_SP1401

    if (en) {
        sp1401->arb_start();
    } else {
        sp1401->arb_stop();
    }

    return 0;
}

int32_t sp3301::set_arb_cnt(uint32_t index,int cnt)
{
    return 0;
}

int32_t sp3301::set_rx_en_tc(const uint32_t index,const bool en)
{
    DECL_DYNAMIC_SP1401
    sp1401->set_rx_en_tc(en);
    return 0;
}

int32_t sp3301::set_rx_level(uint32_t index,double level)
{
    DECL_DYNAMIC_SP1401
    DECL_DYNAMIC_SP2401
    DECL_RF_VER

    uint64_t freq = m_rx_freq[index];
    io_mode_t mode = io_mode_t::_from_integral(m_io_mode[index]);

    switch (RF_VER) {
//        case R1A : case R1B : {
//            r1a::rx_lna_att_t lna_att = r1a::RX_ATT;
//            double att1 = 0.0;
//            int32_t att2 = 0;
//            int64_t ad_0dbfs = g_0dBFS;

//            CAL_FILE_R1A->m_rx_ref->get(freq,level,mode,ad_0dbfs,lna_att,att1,att2);
//            INT_CHECK(SP1401_R1A->set_rx_lna_att_sw(lna_att));
//            INT_CHECK(SP1401_R1A->set_rx_att(att1,att2));
//            INT_CHECK(sp2401->set_rx_pwr_comp(int32_t(g_0dBFS - ad_0dbfs)));
//            break;
//        }
//        case R1C : case R1D : case R1E : case R1F : {
//            rx_ref_op_table_r1cd::rx_state_m_t rx_state;
//            int32_t offset = 0;
//            const bool en_tc = sp1401->is_rx_en_tc();
//            double temp_call = 0.0;
//            double temp_curr = 0.0;
//            double temp_comp = 0.0;
//            double *coef = nullptr;

//            if (mode._to_integral() == io_mode_t::OUTPUT) {
//                CAL_FILE_R1C->m_rx_ref_op->get(RF_VER,freq,level,&rx_state);
//                offset = CAL_FILE_R1C->m_rx_att_op->get(RF_VER,freq,level);
//                offset += rx_state.ad_offset;
//            } else if (mode._to_integral() == io_mode_t::IO) {
//                CAL_FILE_R1C->m_rx_ref_io->get(RF_VER,freq,level,&rx_state);
//                offset = CAL_FILE_R1C->m_rx_att_io->get(RF_VER,freq,level);
//                offset += rx_state.ad_offset;
//            }

//            if (en_tc && (freq >= 3000000000 || rx_state.lna_att == r1c::RX_LNA)) {
//                if (((sp1401_r1c *)sp1401)->get_rx_avg_temp(temp_curr) == true) {
//                    temp_call = rx_state.temp;
//                    temp_curr = temp_curr >= temp_cal_star ? temp_curr : temp_cal_star;
//                    temp_curr = temp_curr <= temp_cal_stop ? temp_curr : temp_cal_stop;

//                    coef = rx_tc_coef[SERIE_INDEX(linear_quantify<uint64_t>(tx_freq_star,FREQ_M(500),freq),tx_freq_star,FREQ_M(500))];
//                    temp_comp = polynomial<double>(coef,5,temp_call) - polynomial<double>(coef,5,temp_curr);
//                    offset = g_0dBFS - dBc_to_ad(g_0dBFS,temp_comp - ad_to_dBc(g_0dBFS,g_0dBFS - offset));
//                }
//            }

//            INT_CHECK(SP1401_R1C->set_rx_lna_att_sw(r1c::rx_lna_att_t(rx_state.lna_att)));
//            INT_CHECK(SP1401_R1C->set_rx_att_019_sw(r1c::rx_att_019_t(rx_state.att_019)));
//            INT_CHECK(SP1401_R1C->set_rx_att(rx_state.att1,rx_state.att2,rx_state.att3));
//            INT_CHECK(sp2401->set_rx_pwr_comp(offset));
//            break;
//        }
        default:break;
    }
    m_ref[index] = level;
    return 0;
}

int32_t sp3301::set_rx_freq(uint32_t index,uint64_t freq)
{
    DECL_DYNAMIC_SP1401
    DECL_DYNAMIC_SP2401

    uint64_t freq_rf = freq / g_rf_freq_space * g_rf_freq_space;
    double freq_ddc = -92640000.0 - double(freq - freq_rf);

    INT_CHECK(sp1401->set_rx_freq(freq_rf));
    INT_CHECK(sp2401->set_ddc(freq_ddc));
    m_rx_freq[index] = freq;
    INT_CHECK(set_rx_level(index,m_ref[index]));
    INT_CHECK(set_rx_bw(index,sp1401->bw()));
    return 0;
}

int32_t sp3301::get_rx_freq(uint32_t index,uint64_t &freq)
{
    freq = m_rx_freq[index];
    return 0;
}

int32_t sp3301::set_rx_bw(uint32_t index,bw_t bw)
{
    DECL_DYNAMIC_SP1401
    DECL_DYNAMIC_SP2401

    switch (sp1401->hw_ver()) {
//        case R1C : case R1D : case R1E : case R1F : {
//            CAL_FILE_R1C->set_bw(bw);
//            double real[ul_filter_tap] = {0.0};
//            double imag[ul_filter_tap] = {0.0};

//            if (_80M == bw) {
//                rx_filter_80m_table::data_m_t data;

//                CAL_FILE_R1C->m_rx_filter_80m->get(m_rx_freq[index],&data);
//                data._2double(real,imag);
//                sp2401->set_rx_filter(real,imag);
//            } else if (_160M == bw) {
//                rx_filter_160m_table::data_m_t data;

//                CAL_FILE_R1C->m_rx_filter_160m->get(m_rx_freq[index],&data);
//                data._2double(real,imag);
//                sp2401->set_rx_filter(real,imag);
//            }
//            return 0;
//        }
        default:return 0;
    }
}

int32_t sp3301::set_io_mode(uint32_t index,io_mode_t mode)
{
    DECL_DYNAMIC_SP1401
    INT_CHECK(sp1401->set_io_mode(mode));
    m_io_mode[index] = mode;
    return 0;
}

int32_t sp3301::set_iq_cap_trig_src(uint32_t index, sp1401::iq_cap_src_t src)
{
    DECL_DYNAMIC_SP1401
    INT_CHECK(sp1401->set_iq_cap_src(src,true));
    return 0;
}

int32_t sp3301::set_iq_cap_trig_level(uint32_t index,float level)
{
    DECL_DYNAMIC_SP1401
    INT_CHECK(sp1401->set_pwr_meas_trig_threshold(double(level)));
    return 0;
}

int32_t sp3301::set_iq_cap_samples(uint32_t index,uint32_t samples)
{
    DECL_DYNAMIC_DDR
    return ddr->set_fpga_w_samples(samples);
}

int32_t sp3301::get_iq_cap_samples(const uint32_t index,uint32_t &samples) const
{
    DECL_DYNAMIC_DDR;
    return ddr->get_fpga_w_samples(samples);
}

int32_t sp3301::set_iq_cap_buffer(uint32_t index,int16_t *I,int16_t *Q)
{
    _I[index] = I;
    _Q[index] = Q;
    return 0;
}

int32_t sp3301::get_iq_cap_buffer(uint32_t index,int16_t **I,int16_t **Q)
{
    *I = _I[index];
    *Q = _Q[index];
    return 0;
}

int32_t sp3301::iq_cap(uint32_t index)
{
    DECL_DYNAMIC_DDR
    INT_CHECK(ddr->fpga_w());
    return 0;
}

int32_t sp3301::iq_cap_abort(uint32_t index)
{
    DECL_DYNAMIC_DDR
    INT_CHECK(ddr->fpga_w_abort());
    return 0;
}

int32_t sp3301::iq_cap_iq2buf(uint32_t index)
{
    DECL_DYNAMIC_DDR
    uint32_t index_2 = brother_idx(index);

    return ddr->iq2buf(index,_I[index],_Q[index],_I[index_2],_Q[index_2]);
}

int32_t sp3301::iq_cap_iq2buf(uint32_t index,uint32_t samples)
{
    DECL_DYNAMIC_DDR
    uint32_t index_2 = brother_idx(index);

    return ddr->iq2buf(index,_I[index],_Q[index],samples,_I[index_2],_Q[index_2]);
}

int32_t sp3301::init_pwr_meas(uint32_t index)
{
    DECL_DYNAMIC_SP1401
    INT_CHECK(sp1401->pwr_meas_abort());
    INT_CHECK(sp1401->pwr_meas_start());
    return 0;
}

int32_t sp3301::abort_pwr_meas(uint32_t index)
{
    DECL_DYNAMIC_SP1401
    INT_CHECK(sp1401->pwr_meas_abort());
    return 0;
}

int32_t sp3301::get_pwr_meas_proc(uint32_t index,sp1401::pwr_meas_state_t &proc)
{
    DECL_DYNAMIC_SP1401
    INT_CHECK(sp1401->get_pwr_meas_state(proc));
    return 0;
}

int32_t sp3301::get_pwr_meas_result(uint32_t index,float &pwr,float &crest)
{
    DECL_DYNAMIC_SP1401

    double pwr_avg = 0.0;
    double pwr_peak = 0.0;
    pwr = -100.0;
    crest = 0.0;

    INT_CHECK(sp1401->get_pwr_meas_pwr(pwr_avg));
    INT_CHECK(sp1401->get_pwr_meas_peak(pwr_peak));
    pwr = float(pwr_avg);
    crest = float(pwr_peak - pwr_avg);
    return 0;
}

int32_t sp3301::get_temp(uint32_t index,double &tx_temp,double &rx_temp)
{
    DECL_DYNAMIC_SP1401

    tx_temp = 0.0;
    rx_temp = 0.0;

    switch (sp1401->hw_ver()) {
        case R1A : case R1B : {
            INT_CHECK(SP1401_R1A->get_tx_temp(tx_temp));
            INT_CHECK(SP1401_R1A->get_rx_temp(rx_temp));
            return 0;
        }
        case R1C : case R1D : case R1E : {
            double tx_t[4] = {0.0,0.0,0.0,0.0};
            double rx_t[4] = {0.0,0.0,0.0,0.0};
            INT_CHECK(SP1401_R1C->get_temp(4,tx_t[0]));
            INT_CHECK(SP1401_R1C->get_temp(5,tx_t[1]));
            INT_CHECK(SP1401_R1C->get_temp(6,tx_t[2]));
            INT_CHECK(SP1401_R1C->get_temp(7,tx_t[3]));
            INT_CHECK(SP1401_R1C->get_temp(0,rx_t[0]));
            INT_CHECK(SP1401_R1C->get_temp(1,rx_t[1]));
            INT_CHECK(SP1401_R1C->get_temp(2,rx_t[2]));
            INT_CHECK(SP1401_R1C->get_temp(3,rx_t[3]));

            for (int32_t i = 0;i < 4;i ++) {
                tx_temp += tx_t[i];
                rx_temp += rx_t[i];
            }
            tx_temp /= 4.0;
            rx_temp /= 4.0;
            return 0;
        }
        case R1F : {
            double tx_t[2] = { 0.0,0.0 };
            double rx_t = 0.0;
            INT_CHECK(SP1401_R1F->get_temp(5,tx_t[0]));
            INT_CHECK(SP1401_R1F->get_temp(6,tx_t[1]));
            INT_CHECK(SP1401_R1F->get_temp(0,rx_t));

            tx_temp = tx_t[0];
            //tx_temp = (tx_t[0] + tx_t[1]) / 2.0;
            rx_temp = rx_t;
            return 0;
        }
        default:return 0;
    }
}

int32_t sp3301::get_cal_temp(uint32_t index, double &temp)
{
    DECL_DYNAMIC_SP1401
    DECL_RF_VER

    switch (RF_VER) {
//        case R1C : case R1D : case R1E : case R1F : {
//            tx_pwr_table_r1c::data_m_t data;
//            CAL_FILE_R1C->m_tx_pwr_op->get_base(tx_freq_star,&data);
//            temp = double(data.temp_5);
//            break;
//        }
        default : { temp = 50.0;}
    }
    return 0;
}

int32_t sp3301::set_iq_cap_frame_trig_src(uint32_t index,sp2401_r1a::frame_trig_src_t src)
{
    DECL_DYNAMIC_SP2401
    INT_CHECK(sp2401->set_frame_trig_src(src));
    return 0;
}

int32_t sp3301::set_iq_cap_frame_trig_frame(uint32_t index,uint32_t frame)
{
    DECL_DYNAMIC_SP2401
    INT_CHECK(sp2401->set_trig_frame(frame));
    return 0;
}

int32_t sp3301::set_iq_cap_frame_trig_offset(uint32_t index,int32_t offset)
{
    DECL_DYNAMIC_SP2401
    INT_CHECK(sp2401->set_frame_trig_offset(offset));
    return 0;
}

int32_t sp3301::set_iq_cap_frame_trig_mod_x_y(uint32_t index,uint16_t x,uint16_t y)
{
    DECL_DYNAMIC_SP2401
    INT_CHECK(sp2401->set_frame_trig_mod_x_y(x,y));
    return 0;
}

int32_t sp3301::instance_sp1401(uint32_t index)
{
    bool is_connected = false;
    char sn[24] = {0};
    hw_ver_t ver = HW_ERROR;

    INT_CHECK(_sp1401[index]->get_sn_major(sn));

    if (sp1401::is_valid_sn(sn) > SN_NULL) {
        is_connected = true;
        sp1401::get_hw_ver(sn,ver);

        switch (ver) {
            case R1A : case R1B : {
                _sp1401[index] = _sp1401_r1a[index];
                break;
            }
            case R1C : case R1D : {
                _sp1401[index] = _sp1401_r1c[index];
                break;
            }
            case R1E : {
                _sp1401[index] = _sp1401_r1e[index];
                break;
            }
            case R1F : case HW_VER_SP9500_MAX : case HW_ERROR : {
                _sp1401[index] = _sp1401_r1f[index];
                break;
            }
        }
        _sp1401[index]->set_hw_ver(ver);
    } else {
        if ((is_connected = _sp1401_r1a[index]->is_connected())) {
            _sp1401[index] = _sp1401_r1a[index];
        } else if ((is_connected = _sp1401_r1c[index]->is_connected())) {
            _sp1401[index] = _sp1401_r1c[index];
        } else if ((is_connected = _sp1401_r1e[index]->is_connected())) {
            _sp1401[index] = _sp1401_r1e[index];
        } else if ((is_connected = _sp1401_r1f[index]->is_connected())) {
            _sp1401[index] = _sp1401_r1f[index];
        } else {
            _sp1401[index] = _sp1401_r1f[index];
        }
    }

    if (is_connected) {
        INT_CHECK(_sp1401[index]->open_board());
        m_active.sp1401[index] = true;
    }
    return 0;
}

int32_t sp3301::self_cal_tx_lol(uint32_t index)
{
    uint64_t rx_freq = 0;
    uint64_t tx_freq = 0;

    INT_CHECK(get_rx_freq(index,rx_freq));
    INT_CHECK(get_tx_freq(index,tx_freq));

    self_cal_tx_lol_helper caller(this,index);

    INT_CHECK(caller.run());

    INT_CHECK(set_rx_freq(index,rx_freq));
    INT_CHECK(set_rx_level(index,0.0));
    INT_CHECK(set_tx_freq(index,tx_freq));
    INT_CHECK(set_tx_pwr(index,-60.0));
    INT_CHECK(set_io_mode(index,io_mode_t::IO));
    return 0;
}

int32_t sp3301::self_cal_tx_sb(uint32_t index)
{
    uint64_t rx_freq = 0;
    uint64_t tx_freq = 0;

    INT_CHECK(get_rx_freq(index,rx_freq));
    INT_CHECK(get_tx_freq(index,tx_freq));

    self_cal_tx_sb_helper caller(this,index);
//    tx_sb_table_r1cd::data_f_t data;

//    INT_CHECK(caller.run(&data));

    INT_CHECK(set_rx_freq(index,rx_freq));
    INT_CHECK(set_rx_level(index,0.0));
    INT_CHECK(set_tx_freq(index,tx_freq));
    INT_CHECK(set_tx_pwr(index,-60.0));
    INT_CHECK(set_io_mode(index,io_mode_t::IO));
    return 0;
}
