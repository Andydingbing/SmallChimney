#include "arb_reader.h"
#include "liblog.h"
#include "integers.hpp"
#include <fmt/core.h>
#include <string>
#include <boost/core/ignore_unused.hpp>

using namespace std;
using namespace ns_arb;
using namespace integers;
using namespace boost;

int32_t arb_reader::load(const char *path)
{
    if (_fp) {
        fclose(_fp);
        _fp = nullptr;
	}

    _fp = fopen(path,"rb");
    if (_fp == nullptr) {
        fclose(_fp);
        Log.set_last_err("Can't open %s",path);
		return -1;
	}
	return 0;
}

FILE *arb_reader::load(param_t *param, std::vector<iq_data_t> *data)
{
    uint32_t file_size = 0;
    uint32_t total_samples = 0;
    header_t file_header;
    list<seg_header_t> seg_header;
    list<seg_header_t>::iterator iter_seg_header;
    list<seg_t>	arb_seg;
    list<marker_list> marker;
    uint32_t marker_idx = 0;

    fseek(_fp,0L,SEEK_END);
    file_size = uint32_t(ftell(_fp));
    fseek(_fp,0L,SEEK_SET);

    fread(&file_header,sizeof(header_t),1,_fp);
    show(&file_header);

    seg_header.clear();
    arb_seg.clear();
    marker.clear();

    for (uint32_t i = 0;i < file_header.segments;++i) {
        seg_header_t each_seg_header;
        fread(&each_seg_header,sizeof(seg_header_t),1,_fp);
        show(&each_seg_header);
        seg_header.push_back(each_seg_header);

        total_samples += each_seg_header.samples;

        uint32_t num_markers = each_seg_header.marker_bytes / sizeof(marker_t);
        marker_t each_marker;
        marker_list each_seg_marker_list;
        seg_t each_seg_arb_seg;

        each_seg_arb_seg.inter_factor = uint32_t(245760000.0f / each_seg_header.sr);
        each_seg_arb_seg.idx          = i;
        each_seg_arb_seg.samples      = each_seg_header.samples;
        each_seg_arb_seg.seg_adr      = i;
        each_seg_arb_seg.sr           = uint32_t(each_seg_header.sr);

        for (uint32_t j = 0;j < num_markers;j ++) {
            fread(&each_marker,sizeof(marker_t),1,_fp);
            each_seg_marker_list.push_back(each_marker);

            marker_idx = each_marker.idx - 1;
            if (each_marker.type == marker_type_t::Period) {
                each_seg_arb_seg.markers[marker_idx].type = 0;
                each_seg_arb_seg.markers[marker_idx].marker.period.start  = each_marker.marker.period.start;
                each_seg_arb_seg.markers[marker_idx].marker.period.period = each_marker.marker.period.period;
                each_seg_arb_seg.markers[marker_idx].marker.period.keep   = each_marker.marker.period.duration;
            } else if (each_marker.type == marker_type_t::UnPeriod) {
                each_seg_arb_seg.markers[marker_idx].type = 1;
                each_seg_arb_seg.markers[marker_idx].marker.unperiod.cnt = each_marker.marker.unperiod.cnt;
                for (uint32_t i =0;i < each_marker.marker.unperiod.cnt;i ++){
                    each_seg_arb_seg.markers[marker_idx].marker.unperiod.start[i] = each_marker.marker.unperiod.start[i];
                    each_seg_arb_seg.markers[marker_idx].marker.unperiod.keep[i]  = each_marker.marker.unperiod.duration[i];
				}
			}
		}
        arb_seg.push_back(each_seg_arb_seg);
        marker.push_back(each_seg_marker_list);
        show(&each_seg_marker_list);
	}

    if (param != nullptr) {
        param->size       = file_size;
        param->samples    = total_samples;
        param->header     = file_header;
        param->seg_header = seg_header;
        param->seg        = arb_seg;
        param->marker     = marker;
	}

    if (data != nullptr) {
        iq_data_t iq_data;

        for (uint32_t i = 0;i < total_samples;++i) {
            fread(&iq_data,sizeof(iq_data_t),1,_fp);
            data->push_back(iq_data);
        }
	}
    return _fp;
}

void arb_reader::show(const header_t *p)
{
    if (p == nullptr) {
        return;
    }

    Log.stdprintf("**************************** file header ****************************\n");
    Log.stdprintf("file_name = %s\n",p->file_name);
    Log.stdprintf("date      = %s\n",p->date);
    Log.stdprintf("version   = %s\n",p->version);
    Log.stdprintf("segments  = %d\n",p->segments);
    Log.stdprintf("checksum  = 0x%x\n",p->checksum);
    Log.stdprintf("*********************************************************************\n");
}

void arb_reader::show(const seg_header_t *p)
{
    if (p == nullptr) {
        return;
    }

    Log.stdprintf("************************** segment header ***************************\n");
    Log.stdprintf("samples      = %d\n",p->samples);
    Log.stdprintf("duration     = %d\n",p->duration);
    Log.stdprintf("level_offset = %f\n",p->level_offset);
    Log.stdprintf("peak_offset  = %f\n",p->peak_offset);
    Log.stdprintf("auth_info    = %s\n",p->auth_info);
    Log.stdprintf("name         = %s\n",p->name);
    Log.stdprintf("marker_bytes = %d\n",p->marker_bytes);
    Log.stdprintf("sr           = %f\n",p->sr);
    Log.stdprintf("*********************************************************************\n");
}

void arb_reader::show(const marker_list *marker)
{
    Log.stdprintf("****************************** marker *******************************\n");
    marker_list::const_iterator iter;

    for (iter = marker->begin();iter != marker->end();++iter) {
        Log.stdprintf("idx  = %d\n",iter->idx);
        Log.stdprintf("type = %s\n",marker_type_t::_from_integral(iter->type)._to_string());
        if (iter->type == marker_type_t::Period) {
            Log.stdprintf("start    = %d\n",iter->marker.period.start);
            Log.stdprintf("duration = %d\n",iter->marker.period.duration);
            Log.stdprintf("period   = %d\n",iter->marker.period.period);
        } else if (iter->type == marker_type_t::UnPeriod) {
            for (uint32_t i = 0;i < iter->marker.unperiod.cnt;++i) {
                Log.stdprintf("start    = %d\n",iter->marker.unperiod.start[i]);
                Log.stdprintf("duration = %d\n",iter->marker.unperiod.duration[i]);
            }
        } else {
            Log.stdprintf("Unknown marker type");
        }
    }
    Log.stdprintf("*********************************************************************\n");
}

void arb_reader::show(const iq_data_t *p)
{
    Log.stdprintf("data.i = %d, data.q = %d\n",p->i,p->q);
}

void arb_reader::get_file_info(string &info)
{
    list<seg_header_t>::iterator iter_seg_header;
    list<marker_list>::iterator iter_marker_list;
    param_t param;

    load(&param);

    iter_seg_header  = param.seg_header.begin();
    iter_marker_list = param.marker.begin();

    info += fmt::format("File Size = {:d}\r\n",param.size);
    info += "********** File Header **********\r\n";
    info += fmt::format("File Name = {:s}\r\n",param.header.file_name);
    info += fmt::format("Date = {:s}\r\n",     param.header.date);
    info += fmt::format("Version = {:s}\r\n",  param.header.version);
    info += fmt::format("Segments = {:s}\r\n", param.header.segments);
    info += fmt::format("Checksum = {:0X}\r\n",param.header.checksum);
    info += "*********************************\r\n";

    uint32_t seg_idx = 0;
    for (iter_seg_header = param.seg_header.begin();iter_seg_header != param.seg_header.end();++iter_seg_header) {
        uint32_t num_markers = iter_seg_header->marker_bytes / sizeof(marker_t);

        info += fmt::format("Segment:{:d}\r\n",        seg_idx);
        info += fmt::format(" Samples = {:d}\r\n",     iter_seg_header->samples);
        info += fmt::format(" Duration = {:d}\r\n",    iter_seg_header->duration);
        info += fmt::format(" Level Offset = {:f}\r\n",iter_seg_header->level_offset);
        info += fmt::format(" Peak  Offset = {:f}\r\n",iter_seg_header->peak_offset);
        info += fmt::format(" Author = {:s}\r\n",      iter_seg_header->auth_info);
        info += fmt::format(" Name = {:s}\r\n",        iter_seg_header->name);
        info += fmt::format(" Marker Bytes = {:d}\r\n",iter_seg_header->marker_bytes);
        info += fmt::format(" Makers = {:d}\r\n",      num_markers);
        info += fmt::format(" SR = {:f}\r\n",          iter_seg_header->sr);

        marker_list::const_iterator iter;

        for (iter = iter_marker_list->begin();iter != iter_marker_list->end();iter ++) {
            info += fmt::format("   Marker {:d}:\r\n",iter->idx);
            info += fmt::format("    Type = {:s}\r\n",marker_type_t::_from_integral(iter->type)._to_string());
            if (iter->type == marker_type_t::Period) {
                info += fmt::format("    Start    = {:d}\r\n",iter->marker.period.start);
                info += fmt::format("    Duration = {:d}\r\n",iter->marker.period.duration);
                info += fmt::format("    Period   = {:d}\r\n",iter->marker.period.period);
            } else if (iter->type == marker_type_t::UnPeriod) {
                for (uint32_t i = 0;i < iter->marker.unperiod.cnt;++i) {
                    info += fmt::format("    Start    = {:d}\r\n",iter->marker.unperiod.start[i]);
                    info += fmt::format("    Duration = {:d}\r\n",iter->marker.unperiod.duration[i]);
				}
            } else {
                info += "    Unknown Marker Type\r\n";
            }
		}
        ++seg_idx;
        ++iter_marker_list;
    }
}


int32_t arb_producer_file::load(const char *arg,ns_arb::param_t &param)
{
    INT_CHECK(_file.load(arg));
    _file.load(&param);
    return 0;
}

void arb_producer_file::produce(const uint32_t begin,const uint32_t samples,uint32_t *buf)
{
    ignore_unused(begin);
    fread(buf,sizeof(uint32_t),samples,_file.fp());
}


int32_t arb_producer_const::load(const char *arg,param_t &param)
{
    ns_arb::seg_header_t seg_header;
    ns_arb::seg_t seg;

    normal_notation(string(arg),param.samples);
    param.header.segments = 1;

    seg_header.id = 0;
    seg_header.samples = param.samples;
    seg_header.level_offset = 0.0f;
    seg_header.peak_offset  = 0.0f;
    seg_header.marker_bytes = 0;
    seg_header.sr = 491.52e6f;

    seg.idx = seg_header.id;
    seg.samples = seg_header.samples;
    seg.seg_adr = seg_header.id;
    seg.inter_factor = 0;
    seg.sr = uint32_t(seg_header.sr);

    param.seg_header.push_back(seg_header);
    param.seg.push_back(seg);

    return 0;
}

void arb_producer_const::produce(const uint32_t begin,const uint32_t samples,uint32_t *buf)
{
    ignore_unused(begin);

    for (uint32_t i = 0;i < samples;++i) {
        buf[i] = _value;
    }
}


int32_t arb_producer_increment::load(const char *arg,param_t &param)
{
    ns_arb::seg_header_t seg_header;
    ns_arb::seg_t seg;

    normal_notation(string(arg),param.samples);
    param.header.segments = 1;

    seg_header.id = 0;
    seg_header.samples = param.samples;
    seg_header.level_offset = 0.0f;
    seg_header.peak_offset  = 0.0f;
    seg_header.marker_bytes = 0;
    seg_header.sr = 491.52e6f;

    seg.idx = seg_header.id;
    seg.samples = seg_header.samples;
    seg.seg_adr = seg_header.id;
    seg.inter_factor = 0;
    seg.sr = uint32_t(seg_header.sr);

    param.seg_header.push_back(seg_header);
    param.seg.push_back(seg);

    return 0;
}

void arb_producer_increment::produce(const uint32_t begin,const uint32_t samples,uint32_t *buf)
{
    for (uint32_t i = 0;i < samples;++i) {
        buf[i] = begin + i;
    }
}


arb_mgr::arb_mgr()
{
    _producer.push_back(new arb_producer_file);
    _producer.push_back(new arb_producer_const);
    _producer.push_back(new arb_producer_increment);
}

arb_mgr::~arb_mgr()
{
    for (size_t i = 0;i < _producer.size();++i) {
        delete _producer[i];
        _producer[i] = nullptr;
    }
}
