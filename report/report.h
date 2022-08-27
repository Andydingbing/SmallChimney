#ifndef RD_COMMON_CAL_FILE_H
#define RD_COMMON_CAL_FILE_H

#include "item_table.hpp"
#include "report_header.h"
#include "item_table.hpp"
#include "traits.hpp"
#include <list>
#include <map>

template<typename item_table_t>
class basic_report : noncopyable
{
public:
    basic_report() { _ver_current = 0; }
    basic_report(const std::string &path) { _path = path; }

    void set_path(const std::string &path) { _path = path; }
    void set_name(const std::string &name);

    uint32_t version_current() const { return _ver_current; }
    void set_version_current(const uint32_t ver) { _ver_current = ver; }

    const item_table_base *db(const item_table_t &table) const;
    const item_table_base *db(const int32_t table) const;

#define DB_ITEM_TABLE(children,base) static_cast<const children *>(base)
#define DECLARE_DB_ITEM_TABLE(ptr,children,base) const children *ptr = static_cast<const children *>(base)

public:
    int32_t make_sure_has(const item_table_t table,item_table_base *t);

    int32_t prepare(const item_table_t table);

    int32_t prepare(const item_table_t table,const std::set<uint64_t> &freqs,const bool exp = false);

    int32_t add(const item_table_t table,void *data);

    template<typename int_t,typename traits_t = traits_int<int_t>>
    int32_t add(const int_t table,void *data)
    { return add(item_table_t::_from_integral(table),data); }

    int32_t persistence(const item_table_t table) const;

    template<typename int_t,typename traits_t = traits_int<int_t>>
    int32_t persistence(const int_t table) const
    { return persistence(item_table_t::_from_integral(table)); }

protected:
    virtual int32_t open();

    int32_t slice_and_move(uint32_t pos,uint32_t offset) const;

    void show_header(const report_header_t &head);

private:
    bool pre_check();

    int32_t create();

    int32_t load();

    int32_t load_to_buf(const item_table_t table);

    int32_t table_pos_size(const item_table_t table,uint32_t &pos,uint32_t &size) const;

    int32_t get_header(report_header_t &header) const;

    int32_t get_header(item_header_t &item_header) const;

    int32_t get_header(report_header_t &header,std::list<item_header_t> &item_headers) const;

    int32_t set_header(const report_header_t &header) const
    { return w_from_pos(0,sizeof(report_header_t),&header); }

    int32_t set_header(const item_header_t &item_header) const;

    int32_t set_header(const report_header_t &header,const std::list<item_header_t> &item_headers) const;

    uint32_t get_version() const;

    int32_t w_from_pos(uint32_t pos,uint32_t size,const void *data) const;

    int32_t r_from_pos(uint32_t pos,uint32_t size,void *data) const;

private:
    std::string _path;
    std::map<int32_t,item_table_base *> _tables;

protected:
    uint32_t _ver_current;
    static const int32_t size_granularity = 256;
};

#include "report.inl"

#endif
