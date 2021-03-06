#include "frontend.h"
#include "common/ftp.h"
#include <boost/filesystem/path.hpp>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

frontend::frontend()
{
    _rf_idx = 0;
    _rfu_idx = 0;

    _tx_freqs.push_back(uint64_t());
    _rx_freqs.push_back(uint64_t());

    _en_tx_tc = false;
    _en_rx_tc = false;
}

frontend::frontend(uint32_t rf_idx,uint32_t rfu_idx)
{
    _rf_idx = rf_idx;
    _rfu_idx = rfu_idx;

    _tx_freqs.push_back(uint64_t());
    _rx_freqs.push_back(uint64_t());

    _en_tx_tc = false;
    _en_rx_tc = false;
}

int32_t frontend::open_board()
{
    list<uint64_t>::iterator iter_freq;

    for (iter_freq = _tx_freqs.begin();iter_freq != _tx_freqs.end();++iter_freq) {
        *iter_freq = 0;
    }
    for (iter_freq = _rx_freqs.begin();iter_freq != _rx_freqs.end();++iter_freq) {
        *iter_freq = 0;
    }
    return 0;
}

int32_t frontend::get_ctrller_ver(const std::string &des,uint32_t &ver)
{
    ignore_unused(des,ver);
    return 0;
}

int32_t frontend::set_io_mode(const io_mode_t mode)
{
    ignore_unused(mode);
    return 0;
}

io_mode_t::_integral frontend::io_mode() const
{
    return io_mode_t::OUTPUT;
}

int32_t frontend::set_tx_freq(const uint64_t freq)
{
    ignore_unused(freq);
    return 0;
}

uint64_t frontend::tx_freq()
{
    return *(_tx_freqs.begin());
}

std::string frontend::tx_freq_string()
{
    return freq_string_from_uint64_t(tx_freq());
}

int32_t frontend::set_rx_freq(const uint64_t freq)
{
    ignore_unused(freq);
    return 0;
}

uint64_t frontend::rx_freq()
{
    return *(_rx_freqs.begin());
}

std::string frontend::rx_freq_string()
{
    return freq_string_from_uint64_t(rx_freq());
}
