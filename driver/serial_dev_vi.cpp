#include "serial_dev_vi.h"
#include "pci_dev_vi.h"
#include "visa.h"

using namespace std;

serial_dev_vi::serial_dev_vi()
{
    pci_dev_vi::open_default_rm();
    _session = 0;
}

serial_dev_vi::~serial_dev_vi()
{
    if (_session) {
        viClose(_session);
    }
}

int32_t serial_dev_vi::get_devs(std::vector<std::string> &devs)
{
    ViStatus status = VI_SUCCESS;
    ViFindList find_list = 0;
    ViUInt32 ret_cnt = 0;
    ViChar buf[256] = {0};
    ViChar expr[32] = "ASRL?*INSTR";

    devs.clear();

    if ((status = viFindRsrc(pci_dev_vi::get_default_rm(),expr,&find_list,&ret_cnt,buf)) < VI_SUCCESS) {
        return status;
    }

    devs.push_back(buf);

    while (ret_cnt --) {
        if ((status = viFindNext(find_list,buf)) == VI_ERROR_INV_OBJECT) {
            return status;
        }
        devs.push_back(buf);
    }

    devs.pop_back();
    viClose(find_list);

    return 0;
}

int32_t serial_dev_vi::open(const std::string &dev)
{
    bool open = false;

    is_open(open);

    if (open) {
        return 0;
    }

    ViSession session = 0;
    ViStatus status = VI_SUCCESS;

    if ((status = viOpen(pci_dev_vi::get_default_rm(),const_cast<ViRsrc>(dev.c_str()),VI_NULL,VI_NULL,&session)) < VI_SUCCESS) {
        _session = 0;
        return status;
    }

    _session = session;
    return set_timeout(3000);
}

int32_t serial_dev_vi::is_open(bool &open) const
{
    open = (_session != 0);
    return 0;
}

int32_t serial_dev_vi::close()
{
    viClose(_session);
    _session = 0;
    return 0;
}

int32_t serial_dev_vi::set_baud_rate(const uint32_t rate)
{
    return viSetAttribute(_session,VI_ATTR_ASRL_BAUD,rate);
}

int32_t serial_dev_vi::get_baud_rate(uint32_t &rate) const
{
    return viGetAttribute(_session,VI_ATTR_ASRL_BAUD,&rate);
}

int32_t serial_dev_vi::set_flow_ctrl(const serial_dev_flow_ctrl_t &fc)
{
    ViStatus status = VI_SUCCESS;

    if (fc == serial_dev_flow_ctrl_none) {
        return viSetAttribute(_session,VI_ATTR_ASRL_FLOW_CNTRL,VI_ASRL_FLOW_NONE);
    }

    if (fc == serial_dev_flow_ctrl_software) {
        return viSetAttribute(_session,VI_ATTR_ASRL_FLOW_CNTRL,VI_ASRL_FLOW_XON_XOFF);
    }

    if ((status = viSetAttribute(_session,VI_ATTR_ASRL_FLOW_CNTRL,VI_ASRL_FLOW_RTS_CTS)) < VI_SUCCESS) {
        if ((status = viSetAttribute(_session,VI_ATTR_ASRL_FLOW_CNTRL,VI_ASRL_FLOW_DTR_DSR)) < VI_SUCCESS) {
            return status;
        }
    }
    return 0;
}

int32_t serial_dev_vi::get_flow_ctrl(serial_dev_flow_ctrl_t &fc) const
{
    ViStatus status = VI_SUCCESS;
    ViUInt8 flow = 0;

    VI_CHECK(viGetAttribute(_session,VI_ATTR_ASRL_FLOW_CNTRL,&flow));

    fc = serial_dev_flow_ctrl_hardware;

    if (flow == VI_ASRL_FLOW_NONE) {
        fc = serial_dev_flow_ctrl_none;
    }

    if (flow == VI_ASRL_FLOW_XON_XOFF) {
        fc = serial_dev_flow_ctrl_software;
    }
    return 0;
}

int32_t serial_dev_vi::set_parity_bit(const serial_dev_parity_bit_t &parity)
{
    return viSetAttribute(_session,VI_ATTR_ASRL_PARITY,parity);
}

int32_t serial_dev_vi::get_parity_bit(serial_dev_parity_bit_t &parity) const
{
    return viGetAttribute(_session,VI_ATTR_ASRL_PARITY,&parity);
}

int32_t serial_dev_vi::set_stop_bit(const serial_dev_stop_bit_t &stop)
{
    return viSetAttribute(_session,VI_ATTR_ASRL_STOP_BITS,stop * 5 + 10);
}

int32_t serial_dev_vi::get_stop_bit(serial_dev_stop_bit_t &stop) const
{
    ViStatus status = VI_SUCCESS;
    ViUInt8 s = 0;

    VI_CHECK(viGetAttribute(_session,VI_ATTR_ASRL_STOP_BITS,&s));
    stop = serial_dev_stop_bit_t((s - 10) / 5);
    return 0;
}

int32_t serial_dev_vi::set_data_bits(const uint8_t bits)
{
    return viSetAttribute(_session,VI_ATTR_ASRL_DATA_BITS,bits);
}

int32_t serial_dev_vi::get_data_bits(uint8_t &bits) const
{
    return viGetAttribute(_session,VI_ATTR_ASRL_DATA_BITS,&bits);
}

int32_t serial_dev_vi::set_timeout(const uint32_t timeout)
{
    return viSetAttribute(_session,VI_ATTR_TMO_VALUE,timeout > 0 ? timeout : UINT32_MAX);
}

int32_t serial_dev_vi::get_timeout(uint32_t &timeout) const
{
    return viGetAttribute(_session,VI_ATTR_TMO_VALUE,&timeout);
}

int32_t serial_dev_vi::set_terminate_char_en(const bool en)
{
    return viSetAttribute(_session,VI_ATTR_TERMCHAR_EN,en);
}

int32_t serial_dev_vi::get_terminate_char_en(bool &en) const
{
    return viGetAttribute(_session,VI_ATTR_TERMCHAR_EN,&en);
}

int32_t serial_dev_vi::set_terminate_char(const char str)
{
    return viSetAttribute(_session,VI_ATTR_TERMCHAR,str);
}

int32_t serial_dev_vi::get_terminate_char(char &str) const
{
    return viGetAttribute(_session,VI_ATTR_TERMCHAR,&str);
}

int32_t serial_dev_vi::write(const string &data) const
{
    ViUInt32 ret_cnt = 0;

    return viWrite(_session,ViBuf(data.c_str()),ViUInt32(data.length()),&ret_cnt);
}

int32_t serial_dev_vi::read()
{
    const ViUInt32 length = 512;
    ViStatus status = VI_SUCCESS_MAX_CNT;
    ViUInt32 ret_cnt = 0;
    ViByte read_buf[length] = {0};

    while (status == VI_SUCCESS_MAX_CNT) {
        status = viRead(_session,read_buf,length,&ret_cnt);
    }

    return status;
}

int32_t serial_dev_vi::read(std::string &buf)
{
    const ViUInt32 length = 512;
    ViStatus status = VI_SUCCESS_MAX_CNT;
    ViUInt32 ret_cnt = 0;
    ViByte read_buf[length] = {0};

    while (status == VI_SUCCESS_MAX_CNT) {
        status = viRead(_session,read_buf,length,&ret_cnt);
        buf.append(read_buf,read_buf + ret_cnt);
    }

    return status;
}

int32_t serial_dev_vi::set_read_end_mode(const serial_dev_read_end_mode_t &method)
{
    return viSetAttribute(_session,VI_ATTR_ASRL_END_IN,method);
}

int32_t serial_dev_vi::get_read_end_mode(serial_dev_read_end_mode_t &method) const
{
    return viGetAttribute(_session,VI_ATTR_ASRL_END_IN,&method);
}
