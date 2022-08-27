#ifndef FRONTEND_H
#define FRONTEND_H

#include "global_header.h"
#include "freq_string.hpp"
#include <list>
#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>
#include <boost/core/ignore_unused.hpp>

BETTER_ENUM(io_mode_t, int32_t, IO, OUTPUT, LOOP, INPUT, CLOSE)

struct chain_state_t
{
    int32_t *states;
    uint32_t n;
};

chain_state_t* chain_state_new();
int32_t chain_state_init(chain_state_t *chain_state);
int32_t chain_state_to_uint64(chain_state_t *chain_state,uint32_t offset,uint64_t *out);
int32_t chain_state_from_uint64(chain_state_t *chain_state,uint32_t offset,uint64_t in);
int32_t chain_state_del(chain_state_t *chain_state);

class API frontend : noncopyable
{
public:
    enum rf_ch_t { CH_TX = 0, CH_RX = 1 };

public:
    frontend();
    frontend(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~frontend() {}

    uint32_t rf_idx() const { return _rf_idx; }
    uint32_t rfu_idx() const { return _rfu_idx; }

    void set_rf_idx(const uint32_t idx)  { _rf_idx = idx; }
    void set_rfu_idx(const uint32_t idx) { _rfu_idx = idx; }

    virtual bool is_connected() { return false; }

    virtual int32_t open_board();
    virtual int32_t close_board() { return 0; }

    virtual int32_t get_ctrller_ver(const std::string &des,uint32_t &ver);

    virtual int32_t hw_ver() const { return -1; }

    virtual int32_t get_sn_major(std::string &sn) { boost::ignore_unused(sn); return 0; }

    virtual int32_t set_io_mode(const io_mode_t mode);
    virtual io_mode_t::_integral io_mode() const;

    virtual int32_t set_tx_freq(const uint64_t freq);
    virtual uint64_t tx_freq();

    virtual int32_t set_rx_freq(const uint64_t freq);
    virtual uint64_t rx_freq();

public:
    std::string tx_freq_string();
    std::string rx_freq_string();

    void set_tx_en_tc(const bool en) { _en_tx_tc = en; }
    void set_rx_en_tc(const bool en) { _en_rx_tc = en; }
    bool is_tx_en_tc() const { return _en_tx_tc; }
    bool is_rx_en_tc() const { return _en_rx_tc; }

public:
    uint32_t _rf_idx;
    uint32_t _rfu_idx;

    std::list<uint64_t> _tx_freqs;
    std::list<uint64_t> _rx_freqs;

    bool _en_tx_tc;
    bool _en_rx_tc;
};

#endif
