#include "report.h"
#include "memory.h"
#include "traits.hpp"
#include <list>
#include <map>
#include <fstream>
#include "liblog.h"
#include <boost/current_function.hpp>

#define assert_fs(stream) \
    do {if ((stream).bad()) { \
        Log.set_last_err("%s:%s:%d",__FILE__,BOOST_CURRENT_FUNCTION,__LINE__); \
        return -1; \
        } \
    } while(0)

#define open_to_w(stream) \
    std::ofstream stream(_path,std::ofstream::binary); \
    assert_fs(stream);

#define open_to_r(stream) \
    std::ifstream stream(_path,std::ifstream::binary); \
    assert_fs(stream);

#define open_to_wr(stream) \
    std::fstream stream(_path,std::ios::in | std::ios::out | std::ios::binary); \
    assert_fs(stream);

#define stream_r(dst) \
    assert_fs(stream.read(reinterpret_cast<char *>(&dst),sizeof(dst)));

#define stream_w(src) \
    assert_fs(stream.write(reinterpret_cast<const char *>(&src),sizeof(src)));

template<typename item_table_t>
void basic_report<item_table_t>::set_name(const std::string &name)
{
    std::string suffix = ".cal";
    size_t suffix_length = suffix.length();

    if(_path.length() > suffix_length &&
       _path.substr(_path.length() - suffix_length,suffix_length) == suffix) {
        return;
    }

    _path += name;
    _path += suffix;
}


template<typename item_table_t>
int32_t basic_report<item_table_t>::make_sure_has(const item_table_t table,item_table_base *t)
{
    int32_t data_start_pos = 0;

    report_header_t header;
    item_header_t item_header;
    std::list<item_header_t> item_headers;
    typename std::list<item_header_t>::const_iterator iter_ih;

    item_header.id = table._to_integral();
    item_header.per_size = t->size_of_data_f();

    if ((data_start_pos = get_header(header,item_headers)) < 0) {
        return 0;
    }

    for (iter_ih = item_headers.begin();iter_ih != item_headers.end();++iter_ih) {
        if ((*iter_ih).id == item_header.id) {
            break;
        }
    }

    if (iter_ih == item_headers.end()) {
        header.tables += 1;
        item_headers.push_back(item_header);

        INT_CHECK(slice_and_move(data_start_pos,sizeof(item_header_t)));
        INT_CHECK(set_header(header,item_headers));
        INT_CHECK(load());
    }

    INT_CHECK(load());


    _tables[table._to_integral()] = t;
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::prepare(const item_table_t table)
{
    std::map<int32_t,item_table_base *>::const_iterator iter;

    load_to_buf(table);

    if ((iter = _tables.find(table._to_integral())) != _tables.end()) {
        iter->second->prepare(convert_buf.buf,convert_buf.size_table);
        return 0;
    }
    return -1;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::prepare(const item_table_t table,const std::set<uint64_t> &freqs,const bool exp)
{
    std::map<int32_t,item_table_base *>::const_iterator iter;

    load_to_buf(table);

    if ((iter = _tables.find(table._to_integral())) != _tables.end()) {
        iter->second->prepare(convert_buf.buf,convert_buf.size_table,&freqs,exp);
        return 0;
    }
    return -1;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::add(const item_table_t table,void *data)
{
    std::map<int32_t,item_table_base *>::const_iterator iter;

    if ((iter = _tables.find(table._to_integral())) != _tables.end()) {
        iter->second->add(data);
        return 0;
    }
    return -1;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::persistence(const item_table_t table) const
{
    uint32_t pos = 0;
    uint32_t cur_size = 0;
    uint32_t new_size = 0;
    uint32_t per_size = 0;
    item_header_t table_header;
    item_table_base *t = nullptr;
    std::string freq_str;

    set_helper::set_helper_t<int64_t,std::vector<int64_t>> freq;

    std::map<int32_t,item_table_base *>::const_iterator iter;

    if ((iter = _tables.find(table._to_integral())) == _tables.end()) {
        return 0;
    }

    INT_CHECK(table_pos_size(table,pos,cur_size));

    t = iter->second;
    t->combine(freq_str);
    per_size = t->size_of_data_f();
    new_size = t->size_data_f() * per_size;
    table_header.id = table._to_integral();

    if (new_size > cur_size) {
        INT_CHECK(get_header(table_header));
        table_header.pts = t->size_data_f();
        table_header.per_size = t->size_of_data_f();
        table_header.set_freq_str(freq_str);
        INT_CHECK(set_header(table_header));
        INT_CHECK(slice_and_move(pos + cur_size,new_size - cur_size));
    }
    for (uint32_t i = 0;i < t->size_data_f();++i) {
        INT_CHECK(w_from_pos(pos,per_size,t->data_f(i)));
        pos += per_size;
    }
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::open()
{
    if (!pre_check()) {
        if (create() < 0) { return -1; }
        if (!pre_check()) { return -1; }
    }
    load();
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::slice_and_move(uint32_t pos,uint32_t offset) const
{
    uint32_t size = 0;
    int32_t left = 0;
    int32_t move_block_size = 0;
    char buf[size_granularity] = {0};

    open_to_wr(stream);
    stream.seekp(0,std::ios::end);
    stream.seekg(0,std::ios::end);
    size = uint32_t(stream.tellp());

    if (pos > size) {
        return 0;
    }

    if (pos == size) {
        left = int32_t(offset);
        while (left > size_granularity) {
            assert_fs(stream.write(buf,size_granularity));
            left -= size_granularity;
        }
        assert_fs(stream.write(buf,left));
        return 0;
    }

    left = int32_t(size - pos);

    while(1) {
        if (left <= 0) {
            break;
        }

        move_block_size = left < size_granularity ? left : size_granularity;
        move_block_size = int32_t(offset) < move_block_size ? int32_t(offset) : move_block_size;

        assert_fs(stream.seekg(move_block_size * -1,std::ios::cur));
        assert_fs(stream.read(buf,move_block_size));
        assert_fs(stream.seekg(move_block_size * -1,std::ios::cur));
        assert_fs(stream.seekg(offset,std::ios::cur));
        assert_fs(stream.write(buf,move_block_size));

        left -= move_block_size;

        assert_fs(stream.seekg(pos + uint32_t(left),std::ios::beg));
    }
    return 0;
}

template<typename item_table_t>
void basic_report<item_table_t>::show_header(const report_header_t &head)
{

}

template<typename item_table_t>
bool basic_report<item_table_t>::pre_check()
{
    uint32_t ver = get_version();

    if (ver == 0) {
        return false;
    } else if (ver < _ver_current) {
        //        cfv_ctrl ctrl;
        //        ctrl.target(this);
        //        ctrl.for_c(ver);
    }
    return true;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::create()
{
    report_header_t header;
    header.ver = 1;

    open_to_w(stream);
    stream_w(header);
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::load()
{
    report_header_t header;
    item_header_t table_header;

    uint32_t max_size = 0;
    uint32_t cur_table_size = 0;
    std::list<item_header_t> table_headers;
    typename std::list<item_header_t>::iterator iter_th;

    std::map<int32_t,item_table_base *>::iterator iter_tables;

    open_to_r(stream);
    stream_r(header);

    for (uint32_t i = 0;i < header.tables;++i) {
        stream_r(table_header);

        cur_table_size = table_header.pts * table_header.per_size;
        if (cur_table_size > max_size) {
            max_size = cur_table_size;
        }
        table_headers.push_back(table_header);
    }

    convert_buf.allocate(max_size);

    for (iter_th = table_headers.begin();iter_th != table_headers.end();++iter_th) {
        cur_table_size = (*iter_th).pts * (*iter_th).per_size;
        assert_fs(stream.read(static_cast<char *>(convert_buf.buf),cur_table_size));
        convert_buf.table = iter_th->id;
        convert_buf.size_table = cur_table_size;

        if ((iter_tables = _tables.find(iter_th->id)) != _tables.end()) {
            if (iter_tables->second->size_of_data_f() != iter_th->per_size) {
                continue;
            }

            iter_tables->second->freq_sequence()->clear();
            iter_tables->second->map_from(convert_buf.buf,(*iter_th).pts);
            sequence::parse(iter_th->freq_str,iter_tables->second->freq_sequence());

            Log.stdprintf("freq string %s : \n",iter_th->freq_str);
        }
    }
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::load_to_buf(const item_table_t table)
{
    std::map<int32_t,item_table_base *>::iterator iter_tables;

    if (convert_buf.table == table) {
        return 0;
    }

    if ((iter_tables = _tables.find(table._to_integral())) == _tables.end()) {
        return -1;
    }

    uint32_t pos = 0;
    uint32_t size = 0;

    table_pos_size(table,pos,size);

    r_from_pos(pos,size,convert_buf.buf);

    convert_buf.table = table;
    convert_buf.size_table = size;
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::table_pos_size(const item_table_t table,uint32_t &pos,uint32_t &size) const
{
    pos = 0;
    size = 0;
    bool ignore = false;
    uint32_t each_table_size = 0;
    report_header_t header;
    item_header_t table_header;

    open_to_r(stream);
    stream_r(header);

    for (uint32_t i = 0;i < header.tables;i ++) {
        stream_r(table_header);

        each_table_size = table_header.pts * table_header.per_size;
        if (table_header.id != table._to_integral()) {
            pos += ignore ? 0 : each_table_size;
        } else {
            size = each_table_size;
            ignore = true;
        }
    }

    pos += uint32_t(stream.tellg());
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::get_header(report_header_t &header) const
{
    open_to_r(stream);
    stream_r(header);
    return int32_t(stream.tellg());
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::get_header(item_header_t &item_header) const
{
    report_header_t header;
    item_header_t each_table_header;

    open_to_wr(stream);
    stream_r(header);

    for (uint32_t i = 0;i < header.tables;++i) {
        stream_r(each_table_header);

        if (each_table_header.id == item_header.id) {
            item_header = each_table_header;
        }
    }
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::get_header(report_header_t &header,std::list<item_header_t> &item_headers) const
{
    item_header_t table_header;

    item_headers.clear();

    open_to_r(stream);
    stream_r(header);

    for (uint32_t i = 0;i < header.tables;i ++) {
        stream_r(table_header);
        item_headers.push_back(table_header);
    }
    return int32_t(stream.tellg());
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::set_header(const item_header_t &item_header) const
{
    report_header_t header;
    item_header_t each_table_header;
    const int32_t table_header_size = sizeof(item_header_t);

    open_to_wr(stream);
    stream_r(header);

    for (uint32_t i = 0;i < header.tables;++i) {
        stream_r(each_table_header);

        if (each_table_header.id == item_header.id) {
            assert_fs(stream.seekp(table_header_size * -1,std::ios::cur));
            stream_w(item_header);
            return 0;
        }
    }
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::set_header(const report_header_t &header,const std::list<item_header_t> &item_headers) const
{
    typename std::list<item_header_t>::const_iterator iter_th = item_headers.cbegin();

    open_to_wr(stream);
    stream_w(header);

    for (;iter_th != item_headers.cend();++iter_th) {
        stream_w(*iter_th);
    }
    return 0;
}

template<typename item_table_t>
uint32_t basic_report<item_table_t>::get_version() const
{
    report_header_t header;
    get_header(header);
    return header.ver;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::w_from_pos(uint32_t pos,uint32_t size,const void *data) const
{
    open_to_wr(stream);
    assert_fs(stream.seekp(pos,std::ios::beg));
    assert_fs(stream.write(static_cast<const char *>(data),size));
    return 0;
}

template<typename item_table_t>
int32_t basic_report<item_table_t>::r_from_pos(uint32_t pos,uint32_t size,void *data) const
{
    open_to_wr(stream);
    assert_fs(stream.seekg(pos,std::ios::beg));
    assert_fs(stream.read(static_cast<char *>(data),size));
    return 0;
}
