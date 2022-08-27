#include "frontend.h"
#include "common/ftp.h"
#include <boost/filesystem/path.hpp>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

chain_state_t* chain_state_new()
{
    chain_state_t *chain_state = (chain_state_t *)(malloc(sizeof(chain_state_t)));

    return chain_state;
}

int32_t chain_state_init(chain_state_t *chain_state)
{
    if (!chain_state) {
        chain_state = chain_state_new();
    }

    if (!chain_state) {
        return -1;
    }

    chain_state->states = (int32_t *)(malloc(chain_state->n * sizeof(int32_t)));

    return chain_state->states == NULL;
}

int32_t chain_state_to_uint64(chain_state_t *chain_state,uint32_t offset,uint64_t *out)
{
    if (!chain_state || chain_state->n < offset) {
        return -1;
    }

    out = (uint64_t *)(&chain_state->states[offset]);
    return 0;
}


int32_t chain_state_from_uint64(chain_state_t *chain_state,uint32_t offset,uint64_t in)
{
    if (!chain_state || chain_state->n < offset) {
        return -1;
    }

    memcpy(&chain_state->states[offset],&in,sizeof(uint64_t));
    return 0;
}

int32_t chain_state_del(chain_state_t *chain_state)
{
    if (!chain_state) {
        return 0;
    }

    if (chain_state->states) {
        free(chain_state->states);
        chain_state->states = NULL;
        chain_state->n = 0;
    }

    free(chain_state);
    return 0;
}

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
