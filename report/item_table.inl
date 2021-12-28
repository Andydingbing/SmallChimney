#include "item_table.hpp"
#include "fmt/core.h"

template<typename data_f_t,typename data_m_t>
int32_t item_table<data_f_t,data_m_t>::get(const uint64_t &key,data_f_t &data) const
{
    typename std::vector<data_f_t>::iterator iter = _data_f->begin();

    for (;iter != _data_f->end();++iter) {
        if (iter->key() >= int64_t(key)) {
            data = *iter;
            return 0;
        }
    }
    return -1;
}

template<typename data_f_t,typename data_m_t>
int32_t item_table<data_f_t,data_m_t>::get(const uint64_t &key,data_m_t &data) const
{
    typename std::vector<data_m_t>::const_iterator iter = _data_m.cbegin();

    for (;iter != _data_m.cend();++iter) {
//      if (iter->key() >= int64_t(key)) {
//          data = *iter;
//          return 0;
//      }
    }
    return 0;
}

template<typename data_f_t,typename data_m_t>
void item_table<data_f_t,data_m_t>::update_data_reception_from_data_f(const std::set<uint64_t> *keys)
{
    if (_data_reception->size() == 0) {
        return;
    }

    size_t i = 0;
    data_f_t data;
    std::set<uint64_t>::iterator iter_key = keys->begin();
    typename std::vector<data_f_t>::iterator iter_data = _data_reception->begin();

    for (;iter_key != keys->end();++iter_key,++iter_data) {
        INT_CHECKV(get(*iter_key,data));

        *iter_data = data;
        iter_data->set_key(*iter_key);

        if (++i == _data_reception->size()) {
            return;
        }
    }
}

template<typename data_f_t,typename data_m_t>
void item_table<data_f_t,data_m_t>::prepare(
        void *data_f_before, uint32_t data_f_before_size,
        const std::set<uint64_t> *keys,
        const bool update_data_reception)
{
    SAFE_NEW(_data_f,std::vector<data_f_t>);
    SAFE_NEW(_data_reception,std::vector<data_f_t>);

    _data_f->clear();
    _data_f->assign(data_f_before_size / size_of_data_f(),data_f_t());
    // _data_f->resize(data_f_before_size / size_of_data_f());

    if (data_f_before != nullptr && data_f_before_size != 0) {
        memcpy(data_f(0),data_f_before,data_f_before_size);
    }

    _data_reception->clear();

    if (keys == nullptr) {
        return;
    }

    std::set<uint64_t>::iterator iter_keys;
    data_f_t data;

    for (iter_keys = keys->begin();iter_keys != keys->end();++iter_keys) {
       if (is_between(int64_t(*iter_keys),data.key_lower_bound(),data.key_upper_bound())) {
            data.set_key(*iter_keys);
            _data_reception->push_back(data);
       }
    }

    if (update_data_reception) {
        update_data_reception_from_data_f(keys);
    }
}

template<typename data_f_t,typename data_m_t>
void item_table<data_f_t,data_m_t>::add(void *data)
{
    data_f_t *d = static_cast<data_f_t *>(data);

    if (_data_reception->empty()) {
        _data_reception->push_back(*d);
        return;
    }

    // Speed up
    if (_data_reception->back().key() == d->key()) {
        (*_data_reception)[size_data_reception() - 1] = *d;
        return;
    }

    typename std::vector<data_f_t>::iterator iter;

    for (iter = _data_reception->begin();iter != _data_reception->end();++iter) {
        if (iter->key() == d->key()) {
            *iter = *d;
            return;
        }
    }

//    Log.stdprintf("wtf!!!\n");
    _data_reception->push_back(*d);
}

template<typename data_f_t,typename data_m_t>
void item_table<data_f_t,data_m_t>::combine(std::string &freq_str)
{
    size_t last_idx = 0;
    bool is_new_element = true;
    std::set<int64_t> freq;
    typename std::vector<data_f_t>::const_iterator iter_data_f;

    if (_data_reception == nullptr || _data_f == nullptr) {
        return;
    }
    for (size_t i = 0;i < _data_reception->size();++i) {
        is_new_element = true;
        for (size_t j = last_idx;j < _data_f->size();++j) {
            if (_data_f->at(j).key() == _data_reception->at(i).key()) {
                (*_data_f)[j] = (*_data_reception)[i];
                last_idx = j;
                is_new_element = false;
                break;
            }
        }
        if (is_new_element) {
            _data_f->push_back(_data_reception->at(i));
        }
    }

    for (iter_data_f = _data_f->cbegin();iter_data_f != _data_f->cend();++iter_data_f) {
        freq.insert(freq.begin(),iter_data_f->freq);
    }
    sequence_string_of<int64_t>(freq,freq_str);
}

template<typename data_f_t,typename data_m_t>
std::string item_table<data_f_t,data_m_t>::data_reception(uint32_t row,uint32_t column) const
{
    uint32_t each_rows = each_data_reception_rows();

    if (each_rows == 0) {
        return "";
    }

    uint32_t key_idx  = each_rows == 0 ? 0 : row / each_rows;
    uint32_t data_idx = each_rows == 0 ? 0 : row % each_rows;

    return each_data_reception(_data_reception->at(key_idx),data_idx,column);
}

template<typename data_f_t,typename data_m_t>
uint32_t item_table<data_f_t,data_m_t>::data_reception_rows() const
{
    if (_data_reception == nullptr) {
        return 0;
    }

    return uint32_t(_data_reception->size() * each_data_reception_rows());
}

template<typename data_f_t,typename data_m_t>
uint32_t item_table<data_f_t,data_m_t>::data_reception_columns() const
{
    return each_data_reception_columns();
}

template<typename data_f_t,typename data_m_t>
uint32_t item_table<data_f_t,data_m_t>::data_reception_points(uint32_t key_idx) const
{
    return _data_reception == nullptr ? 0 : each_data_reception_points(_data_reception->at(key_idx));
}

template<typename data_f_t,typename data_m_t>
double item_table<data_f_t,data_m_t>::data_reception_x(uint32_t key_idx,uint32_t idx) const
{
    return _data_reception == nullptr ? 0 : each_data_reception_x(_data_reception->at(key_idx),idx);
}

template<typename data_f_t,typename data_m_t>
double item_table<data_f_t,data_m_t>::data_reception_y(uint32_t key_idx,uint32_t idx) const
{
    return _data_reception == nullptr ? 0 : each_data_reception_y(_data_reception->at(key_idx),idx);
}
