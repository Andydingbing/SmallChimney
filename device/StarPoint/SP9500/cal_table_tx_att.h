#ifndef DEVICE_STARPOINT_SP9500_CAL_TABLE_TX_ATT_H
#define DEVICE_STARPOINT_SP9500_CAL_TABLE_TX_ATT_H

#include "item_table.hpp"
#include "global_header.h"
#include "../cal_table.h"

namespace ns_starpoint {
namespace ns_sp9500 {

template<int T>
struct data_f_tx_att : item_table_base::data_f_base_t
{
    float  offset[T];
    double temp[4];

    data_f_tx_att()
    {
        ZERO_ARRAY(offset);
        INIT_ARRAY(temp,-100.0);
    }
};

template<int T>
struct data_m_tx_att
{
    float offset[T];
    float temp_5;
};

typedef data_f_tx_att<R1C_TX_ATT_OP_POWER_PTS> data_f_tx_att_op;
typedef data_m_tx_att<R1C_TX_ATT_OP_POWER_PTS> data_m_tx_att_op;

typedef data_f_tx_att<R1C_TX_ATT_IO_POWER_PTS> data_f_tx_att_io;
typedef data_m_tx_att<R1C_TX_ATT_IO_POWER_PTS> data_m_tx_att_io;


template<int size,int start,int stop,int step>
class tx_att_table_t : public item_table<data_f_tx_att<size>,data_m_tx_att<size>>
{
public:
    typedef data_f_tx_att<size> data_f_t;
    typedef data_m_tx_att<size> data_m_t;

    void map_from(void *data,uint32_t pts) OVERRIDE
    {
        data_f_t *d_f = static_cast<data_f_t *>(data);
        data_m_t d_m;

        _data_m.clear();

        for (uint32_t i = 0;i < pts;++i) {
            for (int j = 0;j < size;++j) {
                d_m.offset[j] = d_f[i].offset[j];
            }
            d_m.temp_5 = float(d_f[i].temp[1]);
            _data_m.push_back(d_m);
        }
    }

    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE
    {
        if (column == 0) {
            return fmt::format("{:f}",data.freq / 1000000.0);
        }
        if (column == 1) {
            return fmt::format("{:d}",data.dc_i);
        }
        if (column == 2) {
            return fmt::format("{:d}",data.dc_q);
        }
        if (column == 3) {
            return data.use_sa_or_loopback == 0 ? "Spectrum" : "Loopback";
        }
        if (column == 4) {
            return fmt::format("{:f}",data.pwr);
        }
        if (column == 5) {
            return fmt::format("{:f}",data.temp[0]);
        }
        if (column == 6) {
            return fmt::format("{:f}",data.temp[1]);
        }
        if (column == 7) {
            return fmt::format("{:f}",data.temp[2]);
        }
        if (column == 8) {
            return fmt::format("{:f}",data.temp[3]);
        }
        if (column == 9) {
            return string_of<string>(data.time);
        }
        return "";
    }

    uint32_t each_data_reception_rows() const OVERRIDE { return 1; }
    uint32_t each_data_reception_columns() const OVERRIDE { return 10; }

    void save_as(const std::string &path);
};

typedef tx_att_table_t<R1C_TX_ATT_OP_POWER_PTS,R1C_TX_ATT_OP_POWER_STAR,R1C_TX_ATT_OP_POWER_STOP,R1C_TX_ATT_STEP> tx_att_op_table_t;
typedef tx_att_table_t<R1C_TX_ATT_IO_POWER_PTS,R1C_TX_ATT_IO_POWER_STAR,R1C_TX_ATT_IO_POWER_STOP,R1C_TX_ATT_STEP> tx_att_io_table_t;

} // namespace ns_sp9500
} // namespace ns_starpoint

#endif
