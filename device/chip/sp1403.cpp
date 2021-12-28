#include "sp1403.h"
#include "sp1403_r1a.h"
#include "sp1403_r1b.h"

sp1403::sp1403(uint32_t rf_idx,uint32_t rfu_idx) :
    frontend(rf_idx,rfu_idx),
    _tx_freq(*(_tx_freqs.begin())),
    _rx_freq(*(_rx_freqs.begin()))
{
    _io_mode_tx0 = io_mode_t::CLOSE;
    _io_mode_tx1 = io_mode_t::CLOSE;
    _io_mode_rx  = io_mode_t::CLOSE;
}

int32_t sp1403::open_board()
{
    _io_mode_tx0 = io_mode_t::CLOSE;
    _io_mode_tx1 = io_mode_t::CLOSE;
    _io_mode_rx  = io_mode_t::CLOSE;
    _tx_freq = 0;
    _rx_freq = 0;
    return 0;
}

sp1403_r1a::sp1403_r1a() {}

sp1403_r1b::sp1403_r1b() {}
