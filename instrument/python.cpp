#include "libinstrument.h"
#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE(instrument)
{
    def("init",instr_init);
    def("close",instr_close);
    def("has_sa",instr_has_sa);
    def("has_sg",instr_has_sg);
    def("has_pm",instr_has_pm);
    def("has_ps",instr_has_ps);

    def("sa_reset",instr_sa_reset);
    def("sa_set_cf",instr_sa_set_cf);
    def("sa_set_span",instr_sa_set_span);
    def("sa_set_rbw",instr_sa_set_rbw);
    def("sa_set_vbw",instr_sa_set_vbw);
    def("sa_set_det",instr_sa_set_det);
    def("sa_set_peak_search",instr_sa_set_peak_search);
    def("sa_set_marker",instr_sa_set_marker);
    def("sa_set_cal",instr_sa_set_cal);
    def("sa_set_avg_trace",instr_sa_set_avg_trace);
    def("sa_set_ref",instr_sa_set_ref);
    def("sa_get_ref",instr_sa_get_ref);
    def("sa_get_marker_pwr",instr_sa_get_marker_pwr);
    def("sa_set_en_preamp",instr_sa_set_en_preamp);
    def("sa_get_marker_freq",instr_sa_get_marker_freq);
    def("sa_sweep_once",instr_sa_sweep_once);
//    def("sa_marker_move",instr_sa_marker_move);
//    def("sa_marker_move",instr_sa_marker_move);
    def("sa_set_avg_trace_get_data",instr_sa_set_avg_trace_get_data);
    def("sa_set_mech_att",instr_sa_set_mech_att);
//    def("sa_set_ref_auto",instr_sa_set_ref_auto);
//    def("sa_set_ref_auto",instr_sa_set_ref_auto);
//    def("sa_set_ref_auto",instr_sa_set_ref_auto);

    def("sg_reset",instr_sg_reset);
    def("sg_set_cw",instr_sg_set_cw);
    def("sg_get_cw",instr_sg_get_cw);
    def("sg_set_pl",instr_sg_set_pl);
    def("sg_get_pl",instr_sg_get_pl);
    def("sg_set_output_en",instr_sg_set_output_en);
    def("sg_set_mod_en",instr_sg_set_mod_en);

    def("pm_reset",instr_pm_reset);
    def("pm_set_freq",instr_pm_set_freq);
//    def("pm_get_pwr",instr_pm_get_pwr);
//    def("pm_get_pwr",instr_pm_get_pwr);

    def("ps_reset",instr_ps_reset);
}
