#ifndef INSTR_RS_FSUP_H
#define INSTR_RS_FSUP_H

#include "sa.h"

class rs_fsup : public sa
{
public:
    virtual int32_t	get_default_pri_addr() { return -1; }
    virtual int32_t	get_default_sec_addr() { return -1; }
    std::string descriptor() { return std::string("FSUP"); }

public:
    virtual bool reset() OVERRIDE;
    virtual bool set_cf(double freq) const OVERRIDE;
    virtual bool set_span(double freq) const OVERRIDE;
    virtual bool set_rbw(double freq,bool is_auto) const OVERRIDE;
    virtual bool set_vbw(double freq,bool is_auto) const OVERRIDE;
    virtual bool set_det(det_type_t type) const OVERRIDE;
    virtual bool set_peak_search(peak_search_type_t type) const OVERRIDE;
    virtual bool set_marker(marker_type_t type) const OVERRIDE;
    virtual bool set_cal(cal_type_t type) const OVERRIDE;
    virtual bool set_avg_trace(bool en,uint32_t cnt) const OVERRIDE;
    virtual bool get_trace_avg_cnt(uint32_t &cnt) const OVERRIDE;
    virtual bool set_ref(double ref) const OVERRIDE;
    virtual bool get_ref(double &ref) const OVERRIDE;
    virtual bool get_marker_pwr(double &pwr) const OVERRIDE;
    virtual bool set_en_preamp(bool en) const OVERRIDE;
    virtual bool is_en_preamp(bool &en) const OVERRIDE;
    virtual bool is_en_avg_trace(bool &en) const OVERRIDE;
    virtual bool get_marker_freq(double &freq) const OVERRIDE;
    virtual bool wait_for_continue() const OVERRIDE;
    virtual bool sweep_once() const OVERRIDE;
    virtual bool marker_move(marker_move_type_t type) const OVERRIDE;
    virtual bool marker_move(double freq) const OVERRIDE;
    virtual bool set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data) const OVERRIDE;
    virtual bool set_mech_att(bool is_auto,int32_t att) const OVERRIDE;
};

#endif // INSTR_RS_FSUP_H
