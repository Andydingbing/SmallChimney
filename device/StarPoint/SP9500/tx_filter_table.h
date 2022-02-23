#ifndef TX_FILTER_TABLE_H
#define TX_FILTER_TABLE_H

#include "../cal_table.h"

namespace rd {

// rf frequency response
class API tx_rf_fr_table
{
public:
    struct API data_f_t {
        uint64_t freq;
        double   pwr;
	public:
        data_f_t();
    };

public:
    tx_rf_fr_table(){}
    virtual ~tx_rf_fr_table(){}
public:
    virtual void add(data_f_t *data) = 0;
    virtual void get(uint64_t freq,data_f_t *data) = 0;
    virtual void save_as(const char *path) = 0;
public:
    void map2buf(data_f_t *buf) { m_data_f = buf; }
protected:
    data_f_t *m_data_f;
};

#define DECLARE_TX_RF_FR_TABLE_R1CD \
public: \
    typedef tx_rf_fr_table::data_f_t data_f_t; \
    void virtual add(data_f_t *data); \
    void get(uint64_t freq,data_f_t *data); \
    void save_as(const char *path);

// rf frequency response( cw 100m ~~~ 3100m )
class API tx_rf_fr_0_table_r1cd : public tx_rf_fr_table
{
    DECLARE_TX_RF_FR_TABLE_R1CD
};

// rf frequency response( cw 3100m ~~~ 6g )
class API tx_rf_fr_1_table_r1cd : public tx_rf_fr_table
{
    DECLARE_TX_RF_FR_TABLE_R1CD
};

// if frequency response
class API tx_if_fr_table
{
public:
    struct API data_f_t {
        double freq;
        double pwr;
	public:
        data_f_t();
    };

public:
    tx_if_fr_table(){}
    virtual ~tx_if_fr_table(){}
public:
    void virtual add(data_f_t *data);
    void map2buf(data_f_t *buf) { m_data_f = buf; }
    void get(double freq,data_f_t *data);
    void save_as(const char *path);
private:
    data_f_t *m_data_f;
};

// filter bandwidth 80m/160m
class API tx_filter_table
{
public:
    struct API data_f_t {
        uint64_t freq;
        int16_t  coef_real[ns_sp2401::dl_filter_tap_2i];
        int16_t  coef_imag[ns_sp2401::dl_filter_tap_2i];
        int32_t  rsv_0;
        double   temp[4];
        tm       time;
        int32_t  rsv_1;
    public:
        data_f_t();
    };

    STRUCT_ALIGN_API_S(data_m_t,4)
        coef_complex_t coef[ns_sp2401::dl_filter_tap_2i];
    public:
        void _2double(double *real,double *imag);
    STRUCT_ALIGN_API_E(data_m_t,4)

public:
    virtual void get(uint64_t freq,data_m_t *data) = 0;
    virtual void get(uint64_t freq,data_f_t *data) = 0;
};

#define DECLARE_TX_FILTER_TABLE \
public: \
    typedef tx_filter_table::data_f_t data_f_t; \
    typedef tx_filter_table::data_m_t data_m_t; \
    void get(uint64_t freq,data_m_t *data); \
    void get(uint64_t freq,data_f_t *data);

class API tx_filter_80m_table : public tx_filter_table
{
    DECLARE_TX_FILTER_TABLE
    DECLARE_CAL_ITEM_TABLE(tx_filter_80m_table,ns_sp1401::tx_freqs_called)
};

// 160m
class API tx_filter_160m_table : public tx_filter_table
{
    DECLARE_TX_FILTER_TABLE
    DECLARE_CAL_ITEM_TABLE(tx_filter_160m_table,ns_sp1401::tx_freqs_called)
};

} //namespace rd

#endif
