#ifndef RD_COMMON_CAL_TABLE_HPP
#define RD_COMMON_CAL_TABLE_HPP

#define DECL_CAL_TABLE(project,...) \
    namespace ns_##project { BETTER_ENUM(cal_table_t,int32_t,__VA_ARGS__) }

#include "sequence.hpp"
#include <boost/format.hpp>
#include <vector>
#include <fstream>
#include "set_helper.hpp"
#include "algo_math.hpp"

class API item_table_base
{
public:
    typedef ns_sequence::arithmetic_sequence_t<int64_t> sequence_t;

    struct time_t {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t rsv;
        time_t(): year(0),month(0),day(0),hour(0),minute(0),second(0),rsv(0) {}
    };

    struct data_f_base_t {
        time_t   time;
        uint8_t  is_called;
        uint8_t  method;
        uint16_t station;
        uint32_t staff;
        int64_t  freq;
        int64_t  rsv;

        int64_t key() const { return freq; }

        void set_key(const int64_t key)  { freq = key; }
        void set_key(const uint64_t key) { freq = int64_t(key); }

        // can not be virtual
        SYM_INLINE int64_t key_lower_bound() { return INT64_MIN; }
        SYM_INLINE int64_t key_upper_bound() { return INT64_MAX; }

#define DATA_F_KEY_MIN(min) SYM_INLINE int64_t key_lower_bound() { return int64_t(min); }
#define DATA_F_KEY_MAX(max) SYM_INLINE int64_t key_upper_bound() { return int64_t(max); }
    };

    virtual ~item_table_base() {}

    std::vector<sequence_t> *freq_sequence()
    { return &_freq_sequence; }

    std::string freq_sequence_string()
    { return ns_sequence::parse(_freq_sequence); }

    virtual char *data_f(uint32_t idx) = 0;

    virtual uint32_t size_data_f() = 0;
    virtual uint32_t size_of_data_f() = 0;
    virtual uint32_t size_of_data_m() = 0;
    virtual void prepare(void *data_f_before,uint32_t data_f_before_size,
                             const std::set<uint64_t> *keys = nullptr,
                             const bool update_data_reception = false) = 0;
    virtual void map_from(void *data,uint32_t pts) = 0;
    virtual void add(void *data) = 0;
    virtual void combine(std::string &freq_str) = 0;

    virtual std::string data_reception(uint32_t row,uint32_t column) const { return 0; }
    virtual uint32_t data_reception_rows() const { return 0; }
    virtual uint32_t data_reception_columns() const { return 0; }
    virtual uint32_t data_reception_points(uint32_t key_idx) const { return 0; }
    virtual double   data_reception_x(uint32_t key_idx,uint32_t idx) const { return 0.0; }
    virtual double   data_reception_y(uint32_t key_idx,uint32_t idx) const { return 0.0; }

protected:
    std::vector<sequence_t> _freq_sequence;
};

template<typename str_t>
str_t string_of(const item_table_base::time_t &t)
{
    str_t str;
    boost::format fmt("%d");

    str += (fmt % (t.year + 1900)).str().c_str();
    str += (fmt % (t.month)).str().c_str();
    str += (fmt % (t.day)).str().c_str();
    str += (fmt % (t.hour)).str().c_str();
    str += (fmt % (t.minute)).str().c_str();
    str += (fmt % (t.second)).str().c_str();
    return str;
}


template<typename data_f_t,typename data_m_t>
class item_table : public item_table_base
{
public:
    item_table() : _data_reception(nullptr),_data_f(nullptr) {}

    std::vector<data_f_t> *data_reception() { return _data_reception; }
    std::vector<data_f_t> *data_f() { return _data_f; }
    std::vector<data_m_t> *data_m() { return &_data_m; }

    char *data_f(uint32_t idx)
    { return reinterpret_cast<char *>(&(_data_f->at(idx))); }

    uint32_t size_data_reception() const
    { return uint32_t(_data_reception->size()); }

    uint32_t size_data_f()    { return uint32_t(_data_f->size()); }
    uint32_t size_data_m()    { return uint32_t(_data_m.size()); }
    uint32_t size_of_data_f() { return sizeof(data_f_t); }
    uint32_t size_of_data_m() { return sizeof(data_m_t); }

    virtual int32_t get(const uint64_t &key,data_f_t &data) const;
    virtual int32_t get(const uint64_t &key,data_m_t &data) const;

    void update_data_reception_from_data_f(const std::set<uint64_t> *keys);

    void prepare(void *data_f_before,uint32_t data_f_before_size,
                     const std::set<uint64_t> *keys = nullptr,
                     const bool update_data_reception = false);

    void add(void *data);

    void combine(std::string &freq_str);

    std::string data_reception(uint32_t row,uint32_t column) const;
    uint32_t data_reception_rows() const;
    uint32_t data_reception_columns() const;
    uint32_t data_reception_points(uint32_t key_idx) const;
    double   data_reception_x(uint32_t key_idx,uint32_t idx) const;
    double   data_reception_y(uint32_t key_idx,uint32_t idx) const;

protected:
    virtual std::string each_data_reception(const data_f_t &data,uint32_t row,uint32_t column) const { return ""; }
    virtual uint32_t each_data_reception_rows() const {return 0;}
    virtual uint32_t each_data_reception_columns() const { return 0; }
    virtual uint32_t each_data_reception_points(const data_f_t &data) const { return 0; }
    virtual double   each_data_reception_x(const data_f_t &data,uint32_t idx) const { return 0.0; }
    virtual double   each_data_reception_y(const data_f_t &data,uint32_t idx) const { return 0.0; }

protected:
    std::vector<data_f_t> *_data_reception;
    std::vector<data_f_t> *_data_f;
    std::vector<data_m_t> _data_m;
//    std::array<data_m_t,5> _cache;
};

#include "item_table.inl"

#endif
