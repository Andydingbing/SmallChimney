#ifndef ARB_READER_H
#define ARB_READER_H

#include "global_header.h"
#include "enum.h"
#include <stdio.h>
#include <list>
#include <vector>

namespace ns_arb {
BETTER_ENUM(src_t, uint32_t,
            File,
            Const,
            Increment
            )

BETTER_ENUM(marker_type_t, uint32_t, Period, UnPeriod )

struct header_t {
    char file_name[512];
    char date[512];
    char version[512];
    uint32_t segments;
    uint32_t checksum;
};

struct seg_header_t {
    uint32_t id;
    uint32_t samples;
    uint32_t duration;
    float    level_offset;
    float    peak_offset;
    char     name[512];
    char     auth_info[32];
    uint32_t marker_bytes;
    float    sr;
};

struct seg_t
{
    uint32_t idx;
    uint32_t samples;
    uint32_t seg_adr;
    uint32_t inter_factor;
    uint32_t sr;
    struct marker_t {
        uint32_t type;
        union {
            struct {
                uint32_t start;
                uint32_t keep;
                uint32_t period;
            } period;
            struct {
                uint32_t cnt;
                uint32_t start[1024];
                uint32_t keep[1024];
            } unperiod;
        } marker;
    } markers[4];
};

struct marker_t {
    uint32_t idx;
    uint32_t type;
    union {
        struct {
            uint32_t start;
            uint32_t duration;
            uint32_t period;
        } period;
        struct {
            uint32_t cnt;
            uint32_t start[1024];
            uint32_t duration[1024];
        } unperiod;
    } marker;
};

typedef std::list<marker_t> marker_list;

struct param_t {
    uint32_t size;
    uint32_t samples;
    header_t header;
    std::list<seg_header_t> seg_header;
    std::list<seg_t>        seg;
    std::list<marker_list>  marker;
};

} // namespace ns_arb

union as_reg_t
{
    struct {unsigned sample       : 32;} adr0;
    struct {unsigned seg_adr      : 32;} adr1;
    struct {unsigned inter_factor : 32;} adr2;
    struct {unsigned mkr1_start   : 32;} adr3;
    struct {unsigned mkr1_keep    : 32;} adr4;
    struct {unsigned mkr1_period  : 32;} adr5;
    struct {unsigned mkr2_start   : 32;} adr6;
    struct {unsigned mkr2_keep    : 32;} adr7;
    struct {unsigned mkr2_period  : 32;} adr8;
    struct {unsigned mkr3_start   : 32;} adr9;
    struct {unsigned mkr3_keep    : 32;} adr10;
    struct {unsigned mkr3_period  : 32;} adr11;
    struct {unsigned mkr4_start   : 32;} adr12;
    struct {unsigned mkr4_keep    : 32;} adr13;
    struct {unsigned mkr4_period  : 32;} adr14;
    struct {unsigned sr           : 32;} adr16;
	struct {
        unsigned mkr1_type  : 1;
        unsigned mkr2_type  : 1;
        unsigned mkr3_type  : 1;
        unsigned mkr4_type  : 1;
        unsigned mkr1_onoff : 1;
        unsigned mkr2_onoff : 1;
        unsigned mkr3_onoff : 1;
        unsigned mkr4_onoff : 1;
    } adr17;
	uint32_t data;
};

#define AS_REG(addr) as_reg[addr].adr##addr

class API arb_reader : noncopyable
{
public:
    arb_reader() : _fp(nullptr) {}
    ~arb_reader() { if (_fp != nullptr) fclose(_fp); }

    FILE *fp() { return _fp; }

public:
    int32_t load(const char *path = nullptr);
    FILE* load(ns_arb::param_t *param = nullptr,std::vector<iq_data_t> *data = nullptr);
    bool is_loaded() { return (_fp == nullptr ? false : true); }
    void get_file_info(std::string &info);

private:
    void show(const ns_arb::header_t *p);
    void show(const ns_arb::seg_header_t *p);
    void show(const ns_arb::marker_list *marker);
    void show(const iq_data_t *p);

private:
    FILE *_fp;
};


class API arb_producer : noncopyable
{
public:
    virtual ~arb_producer() {}
    virtual int32_t load(const char *arg,ns_arb::param_t &param) = 0;
    virtual void produce(const uint32_t begin,const uint32_t samples,uint32_t *buf) = 0;
};

class API arb_producer_file : public arb_producer
{
public:
    arb_reader *file() { return &_file; }

    int32_t load(const char *arg,ns_arb::param_t &param);
    void produce(const uint32_t begin,const uint32_t samples,uint32_t *buf);

private:
    arb_reader _file;
};

class API arb_producer_const : public arb_producer
{
public:
    arb_producer_const() : _value(100) {}
    int32_t load(const char *arg,ns_arb::param_t &param);
    void produce(const uint32_t begin,const uint32_t samples,uint32_t *buf);

    uint32_t value() { return _value; }
    void set_value(const uint32_t value) { _value = value; }

private:
    uint32_t _value;
};

class API arb_producer_increment : public arb_producer
{
public:
    int32_t load(const char *arg,ns_arb::param_t &param);
    void produce(const uint32_t begin,const uint32_t samples,uint32_t *buf);
};


class API arb_mgr : noncopyable
{
public:
    typedef ns_arb::src_t src_t;

    arb_mgr();
    ~arb_mgr();
    src_t src() const { return src_t::_from_integral(_src); }
    void set_src(const src_t &src) { _src = src; }

    arb_producer_file *producer_file()
    { return dynamic_cast<arb_producer_file *>(_producer[src_t::File]); }

    arb_reader *file() { return producer_file()->file(); }

    arb_producer_const *producer_const()
    { return dynamic_cast<arb_producer_const *>(_producer[src_t::Const]); }

    uint32_t const_value() { return producer_const()->value(); }

    arb_producer_increment *producer_increment()
    { return dynamic_cast<arb_producer_increment *>(_producer[src_t::Increment]); }

    int32_t load(const char *arg,ns_arb::param_t &param)
    { return _producer[_src]->load(arg,param); }

    void produce(const uint32_t begin,const uint32_t samples,uint32_t *buf)
    { return _producer[_src]->produce(begin,samples,buf); }

private:
    uint32_t _src;
    std::vector<arb_producer *> _producer;
};

#endif


