#ifndef INSTR_PS_H
#define INSTR_PS_H

#include "gpib_dev.h"

class API ps : public gpib_dev
{
public: 
    virtual bool set_output_en(const bool en) const;
    virtual bool get_output_en(bool &en) const;

    virtual bool set_current(const double curr_mA) const;
    virtual bool get_current(double &curr_mA) const;
    virtual bool get_current_measured(double &curr_mA) const;
    virtual bool set_current_max(const double curr_mA) const;
    virtual bool get_current_max(double &curr_mA) const;
    virtual bool set_ocp_en(const bool en) const;
    virtual bool get_ocp_en(bool &en) const;

    virtual bool set_voltage(const double vol_mV) const;
    virtual bool get_voltage(double &vol_mV) const;
    virtual bool get_voltage_measured(double &vol_mV) const;
    virtual bool set_voltage_max(const double vol_mV) const;
    virtual bool get_voltage_max(double &vol_mV) const;

    virtual bool trigger_set_current(const double curr_mA) const;
    virtual bool trigger_get_current(double &curr_mA) const;
};

#endif // INSTR_PS_H
