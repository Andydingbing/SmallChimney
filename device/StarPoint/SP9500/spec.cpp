#include "spec.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;
using namespace ns_starpoint;
using namespace ns_sp9500;
using namespace ns_sp1401;
using namespace boost;
using namespace boost::property_tree;

double spec::test_rf_tx_freq_res_max()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_TX_Freq_Res_Test.Max");
}

double spec::test_rf_tx_freq_res_min()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_TX_Freq_Res_Test.Min");
}

double spec::test_rf_tx_freq_res()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_TX_Freq_Res_Test.FreqRes");
}

double spec::test_if_tx_freq_res_max()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_TX_Freq_Res_Test.Max");
}

double spec::test_if_tx_freq_res_min()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_TX_Freq_Res_Test.Min");
}

double spec::test_if_tx_freq_res()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_TX_Freq_Res_Test.FreqRes");
}

double spec::test_rf_rx_freq_res_max()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_RX_Freq_Res_Test.Max");
}

double spec::test_rf_rx_freq_res_min()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_RX_Freq_Res_Test.Min");
}

double spec::test_rf_rx_freq_res()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RF_RX_Freq_Res_Test.FreqRes");
}

double spec::test_if_rx_freq_res_max()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_RX_Freq_Res_Test.Max");
}

double spec::test_if_rx_freq_res_min()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_RX_Freq_Res_Test.Min");
}

double spec::test_if_rx_freq_res()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("IF_RX_Freq_Res_Test.FreqRes");
}

void spec::test_tx_phase_noise_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);
    freq.star = pt.get<string>("TX_Phase_Noise.FreqStar");
    freq.stop = pt.get<string>("TX_Phase_Noise.FreqStop");
    freq.step = pt.get<string>("TX_Phase_Noise.FreqStep");
}

void spec::test_tx_phase_noise_offset(string &offset)
{
    ptree pt;
    read_ini(path(),pt);
    offset = pt.get<string>("TX_Phase_Noise.Offset");
}

void spec::test_tx_phase_noise_span(string &span)
{
    ptree pt;
    read_ini(path(),pt);
    span = pt.get<string>("TX_Phase_Noise.Span");
}

void spec::test_tx_phase_noise_rbw(string &rbw)
{
    ptree pt;
    read_ini(path(),pt);
    rbw = pt.get<string>("TX_Phase_Noise.RBW");
}

double spec::test_tx_phase_noise(uint64_t freq,double offset)
{
    ptree pt;
    read_ini(path(),pt);

    if (freq <= 2000000000) {
        if (offset <= 1000.0) { return pt.get<double>("TX_Phase_Noise.@1K@50_2000"); }
        if (offset <= 10000.0) { return pt.get<double>("TX_Phase_Noise.@10K@50_2000"); }
        if (offset <= 100000.0) { return pt.get<double>("TX_Phase_Noise.@100K@50_2000"); }
    } else if (freq <= 6000000000) {
        if (offset <= 1000.0) { return pt.get<double>("TX_Phase_Noise.@1K@2000_6000"); }
        if (offset <= 10000.0) { return pt.get<double>("TX_Phase_Noise.@10K@2000_6000"); }
        if (offset <= 100000.0) { return pt.get<double>("TX_Phase_Noise.@100K@2000_6000"); }
    }
    return 0.0;
}

void spec::test_tx_noise_floor_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);
    freq.star = pt.get<string>("TX_Noise_Floor.FreqStar");
    freq.stop = pt.get<string>("TX_Noise_Floor.FreqStop");
    freq.step = pt.get<string>("TX_Noise_Floor.FreqStep");
}

void spec::test_tx_noise_floor_span(string &span)
{
    ptree pt;
    read_ini(path(),pt);
    span = pt.get<string>("TX_Noise_Floor.Span");
}

void spec::test_tx_noise_floor_rbw(string &rbw)
{
    ptree pt;
    read_ini(path(),pt);
    rbw = pt.get<string>("TX_Noise_Floor.RBW");
}

double spec::test_tx_noise_floor(uint64_t freq,double pwr)
{
    ptree pt;
    read_ini(path(),pt);

    if (freq <= 2000000000) {
        if (pwr >= 10.0) { return pt.get<double>("TX_Noise_Floor.@10dBm"); }
        if (pwr >= -40.0) { return pt.get<double>("TX_Noise_Floor.@-40dBm"); }
    } else if (freq <= 6000000000) {
        if (pwr >= 10.0) { return pt.get<double>("TX_Noise_Floor.@10dBm"); }
        if (pwr >= -40.0) { return pt.get<double>("TX_Noise_Floor.@-40dBm"); }
    }
    return 0.0;
}

double spec::cal_tx_lol_up()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_LOLeakage.Up");
}

double spec::cal_tx_lol_down()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_LOLeakage.Down");
}

double spec::cal_tx_sb_up()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Sideband.Up");
}

double spec::cal_tx_sb_down()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Sideband.Down");
}

void spec::cal_tx_filter_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);
    freq.star = pt.get<string>("TX_Filter.FreqStar");
    freq.stop = pt.get<string>("TX_Filter.FreqStop");
    freq.step = pt.get<string>("TX_Filter.FreqStep");
}

double spec::cal_tx_filter_flatness()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Filter.Flatness");
}

double spec::cal_tx_filter_ripple()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Filter.Ripple");
}

void spec::cal_tx_base_pwr_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);
    freq.star = pt.get<string>("TX_Base_Power.FreqStar");
    freq.stop = pt.get<string>("TX_Base_Power.FreqStop");
    freq.step = pt.get<string>("TX_Base_Power.FreqStep");
}

double spec::cal_tx_base_pwr_accuracy()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Base_Power.Accuracy");
}

void spec::cal_tx_pwr_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);
    freq.star = pt.get<string>("TX_Power.FreqStar");
    freq.stop = pt.get<string>("TX_Power.FreqStop");
    freq.step = pt.get<string>("TX_Power.FreqStep");
}

void spec::cal_tx_pwr_pwr(const io_mode_t mode,range_pwr_string &pwr)
{
    ptree pt;
    read_ini(path(),pt);

    if (mode._to_integral() == io_mode_t::OUTPUT) {
        pwr.star = pt.get<string>("TX_Power.PowerStar_Output");
        pwr.stop = pt.get<string>("TX_Power.PowerStop_Output");
        pwr.step = pt.get<string>("TX_Power.PowerStep_Output");
    } else if (mode._to_integral() == io_mode_t::IO) {
        pwr.star = pt.get<string>("TX_Power.PowerStar_IO");
        pwr.stop = pt.get<string>("TX_Power.PowerStop_IO");
        pwr.step = pt.get<string>("TX_Power.PowerStep_IO");
    }
}

double spec::cal_tx_pwr_accuracy()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("TX_Power.Accuracy");
}

void spec::cal_rx_ref_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);
    freq.star = pt.get<string>("RX_Reference.FreqStar");
    freq.stop = pt.get<string>("RX_Reference.FreqStop");
    freq.step = pt.get<string>("RX_Reference.FreqStep");
}

double spec::cal_rx_ref_accuracy()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RX_Reference.Accuracy");
}

void spec::cal_rx_pwr_freq(range_freq_string &freq)
{
    ptree pt;
    read_ini(path(),pt);

    freq.star = pt.get<string>("RX_Power.FreqStar");
    freq.stop = pt.get<string>("RX_Power.FreqStop");
    freq.step = pt.get<string>("RX_Power.FreqStep");
}

void spec::cal_rx_pwr_pwr(const io_mode_t mode,range_pwr_string &pwr)
{
    ptree pt;
    read_ini(path(),pt);

    if (mode._to_integral() == io_mode_t::OUTPUT) {
        pwr.star = pt.get<string>("RX_Power.PowerStar_Output");
        pwr.stop = pt.get<string>("RX_Power.PowerStop_Output");
        pwr.step = pt.get<string>("RX_Power.PowerStep_Output");
    } else if (mode._to_integral() == io_mode_t::IO) {
        pwr.star = pt.get<string>("RX_Power.PowerStar_IO");
        pwr.stop = pt.get<string>("RX_Power.PowerStop_IO");
        pwr.step = pt.get<string>("RX_Power.PowerStep_IO");
    }
}

double spec::cal_rx_pwr_accuracy()
{
    ptree pt;
    read_ini(path(),pt);
    return pt.get<double>("RX_Power.Accuracy");
}

string spec::path()
{
    return boost::filesystem::initial_path<boost::filesystem::path>().string()
            + "/"
            + "RF_Spec.ini";
}
