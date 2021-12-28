#ifndef INSTR_SA_H
#define INSTR_SA_H

#include "gpib_dev.h"
#include "libinstrument.h"

class API sa : public gpib_dev
{
public:
    virtual bool set_mode(const sa_mode_t mode) const;
    virtual bool set_meas(const sa_meas_t meas) const;
    virtual bool set_radio_standard(const sa_radio_standard_t standard) const;

    virtual bool set_window_zoom(const bool is_zoomed) const;
    virtual bool set_window_next() const;
    virtual bool set_window_number(const uint8_t n) const;
    virtual bool get_window_number(uint8_t &n) const;
    virtual bool set_window_fullscreen(const bool is_full) const;
    virtual bool get_window_fullscreen(bool &is_full) const;
    virtual bool set_window_display(const bool is_on) const;
    virtual bool get_window_display(bool &is_on) const;

    virtual bool set_io_reset() const;
    virtual bool set_io_mixer(const io_mixer_t mixer) const;
    virtual bool get_io_mixer(io_mixer_t &mixer) const;
    virtual bool set_io_impedance(const io_impedance_t ohm) const;
    virtual bool get_io_impedance(io_impedance_t &ohm) const;
    virtual bool set_io_coupling(const io_coupling_t coupling) const;
    virtual bool get_io_coupling(io_coupling_t &coupling) const;
    virtual bool set_io_ref_in(const io_ref_in_t ref) const;
    virtual bool get_io_ref_in(io_ref_in_t &ref) const;
    virtual bool set_io_ref_ext_freq(const double freq) const;
    virtual bool get_io_ref_ext_freq(double &freq) const;
    virtual bool set_io_ref_ext_freq_default() const;
    virtual bool set_io_ref_ext_bw(const io_ref_ext_bw_t bw) const;
    virtual bool get_io_ref_ext_bw(io_ref_ext_bw_t &bw) const;

    virtual bool set_cf(double freq) const;
    virtual bool set_span(double freq) const;
    virtual bool set_rbw(double freq,bool is_auto) const;
    virtual bool set_vbw(double freq,bool is_auto) const;
    virtual bool set_det(det_type_t type) const;
    virtual bool set_peak_search(peak_search_type_t type) const;
    virtual bool set_marker(marker_type_t type) const;
    virtual bool set_cal(cal_type_t type) const;
    virtual bool set_avg_trace(bool en,uint32_t cnt) const;
    virtual bool get_trace_avg_cnt(uint32_t &cnt) const;
    virtual bool set_ref(double ref) const;
    virtual bool get_ref(double &ref) const;
    virtual bool get_marker_pwr(double &pwr) const;
    virtual bool set_en_preamp(bool en) const;
    virtual bool is_en_preamp(bool &en) const;
    virtual bool is_en_avg_trace(bool &en) const;
    virtual bool get_marker_freq(double &freq) const;
    virtual bool wait_for_continue() const;
    virtual bool sweep_once() const;
    virtual bool marker_move(marker_move_type_t type) const;
    virtual bool marker_move(double freq) const;
    virtual bool set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data) const;
    virtual bool set_mech_att(bool is_auto,int32_t att) const;

    virtual bool acp_set_offset_freq_stat(const uint32_t idx,const bool en) const;
    virtual bool acp_get_offset_freq_stat(const uint32_t idx,bool &en) const;
    virtual bool acp_set_offset_freq_stat(const std::vector<bool> &stat) const;
    virtual bool acp_get_offset_freq_stat(std::vector<bool> &stat) const;

    virtual bool acp_set_offset_freq(const std::vector<uint64_t> &freq) const;
    virtual bool acp_get_offset_freq(std::vector<uint64_t> &freq) const;
    virtual bool acp_get_result(
            std::vector<double> *dbc_lower = nullptr,
            std::vector<double> *dbc_upper = nullptr,
            std::vector<double> *dbm_lower = nullptr,
            std::vector<double> *dbm_upper = nullptr) const;

public:
    bool set_ref_auto() const;
    bool set_ref_auto(double pwr,double &ref) const;
    bool set_ref_auto(double pwr,double &ref,double &offset) const;
};

#endif // INSTR_SA_H
