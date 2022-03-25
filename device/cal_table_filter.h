#ifndef DEVICE_CAL_TABLE_FILTER_H
#define DEVICE_CAL_TABLE_FILTER_H

#include "item_table.hpp"
#include "global_header.h"
#include "utilities.hpp"

template<int tap,int temp_sensor_size = 0>
struct data_f_filter : item_table_base::data_f_base_t
{
    double temp[temp_sensor_size];
    int16_t real[tap];
    int16_t imag[tap];

    data_f_filter()
    {
        ZERO_ARRAY(real);
        ZERO_ARRAY(imag);
        INIT_ARRAY(temp,-100.0);
    }
};

template<int tap>
struct data_m_filter
{
    iq_data_t coef[tap];
};

template<typename data_f_t,typename data_m_t,int tap>
class filter_table_t : public item_table<data_f_t,data_m_t>
{
public:
    void map_from(void *data,uint32_t pts) OVERRIDE
    {
        data_f_t *d_f = static_cast<data_f_t *>(data);
        data_m_t d_m;

        this->_data_m.clear();

        for (uint32_t i = 0;i < pts;++i) {
            for (uint32_t j = 0;j < tap;++j) {
                d_m.coef[j].i = d_f[i].real[j];
                d_m.coef[j].q = d_f[i].imag[j];
            }
            this->_data_m.push_back(d_m);
        }
    }

    std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const OVERRIDE
    {
        if (column == 0) {
            return fmt::format("{:f}",data.freq / 1000000.0);
        }
        if (column == 1) {
            return string_of<std::string>(data.real,tap,",");
        }
        if (column == 2) {
            return string_of<std::string>(data.imag,tap,",");
        }
        if (column == 3) {
            return fmt::format("{:f}",data.temp[0]);
        }
        if (column == 4) {
            return fmt::format("{:f}",data.temp[1]);
        }
        if (column == 5) {
            return fmt::format("{:f}",data.temp[2]);
        }
        if (column == 6) {
            return fmt::format("{:f}",data.temp[3]);
        }
        if (column == 7) {
            return string_of<std::string>(data.time);
        }
        return "";
    }

    uint32_t each_data_reception_rows() const OVERRIDE { return 1; }
    uint32_t each_data_reception_columns() const OVERRIDE { return 8; }
};

#endif
