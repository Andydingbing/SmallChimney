#include "serial_dev_win32.h"
#include "tchar.h"
#include "setupapi.h"
#include <cfgmgr32.h>

using namespace std;
using namespace ns_serial_dev;

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

int32_t list_ports(vector<string> &ports)
{
    HKEY key = 0;
    TCHAR value[64] = {0};
    TCHAR data[64] = {0};
    char  name[64] = {0};

    DWORD max_value_length = 0;
    DWORD max_data_size = 0;
    DWORD value_length = 0;
    DWORD data_size = 0;
    int   name_length = 0;

    DWORD type = 0;
    DWORD index = 0;

    LSTATUS result;

    if ((result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("HARDWARE\\DEVICEMAP\\SERIALCOMM"),0,KEY_QUERY_VALUE,&key))) {
        return result;
	}

    if ((result = RegQueryInfoKey(key,NULL,NULL,NULL,NULL,NULL,NULL,NULL,&max_value_length,&max_data_size,NULL,NULL))) {
        RegCloseKey(key);
        return result;
    }

    while (1)
	{
        value_length = max_value_length + 1;
        data_size = max_data_size;

        if ((result = RegEnumValue(key,index,value,&value_length,NULL,&type,(LPBYTE)data,&data_size))) {
            RegCloseKey(key);
            return result;
        }

        if (type == REG_SZ) {
        #ifdef UNICODE
            name_length = WideCharToMultiByte(CP_ACP,0,data,-1,NULL,0,NULL,NULL);
            WideCharToMultiByte(CP_ACP,0,data,-1,name,name_length,NULL,NULL);
        #else
            name_length = data_length + 1;
            strcpy(name,data);
        #endif

            ports.push_back(name);
		}

        ZERO_ARRAY(value);
        ZERO_ARRAY(data);
        ZERO_ARRAY(name);
		index++;
	}

	RegCloseKey(key);
    return 0;
}

serial_dev_win32::serial_dev_win32() :
    is_writing(false),
    is_reading(false),
    handle(INVALID_HANDLE_VALUE)
{

}

serial_dev_win32::~serial_dev_win32()
{

}

int32_t serial_dev_win32::get_devs(vector<string> &devs)
{
    return list_ports(devs);
}

int32_t serial_dev_win32::open(const string &dev)
{
    string name = (dev[0] == '\\') ? dev : "\\\\.\\" + dev;

    handle = CreateFileA(name.c_str(),GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,0);
    COMMTIMEOUTS timeout;
    DWORD errors = 0;
    COMSTAT status;

    if (handle == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }

    timeout.ReadIntervalTimeout = 0;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.ReadTotalTimeoutConstant = 0;
    timeout.WriteTotalTimeoutMultiplier = 0;
    timeout.WriteTotalTimeoutConstant = 0;

    if (SetCommTimeouts(handle,&timeout) == 0) {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        return GetLastError();
    }

    if ((write_overlapped.hEvent = CreateEvent(nullptr,true,true,nullptr)) == INVALID_HANDLE_VALUE) {
        CloseHandle(write_overlapped.hEvent);
        return GetLastError();
    }

    if ((read_overlapped.hEvent = CreateEvent(nullptr,true,true,nullptr)) == INVALID_HANDLE_VALUE) {
        CloseHandle(read_overlapped.hEvent);
        return GetLastError();
    }

    if ((wait_overlapped.hEvent = CreateEvent(nullptr,true,true,nullptr)) == INVALID_HANDLE_VALUE) {
        CloseHandle(wait_overlapped.hEvent);
        return GetLastError();
    }

    if (SetCommMask(handle,EV_RXCHAR | EV_ERR) == 0) {
        close_overlapped();
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
        return GetLastError();
    }

    if (ClearCommError(handle,&errors,&status) == 0) {
        return GetLastError();
    }
    return 0;
}

int32_t serial_dev_win32::is_open(bool &open) const
{
    open = (handle != INVALID_HANDLE_VALUE);
    return 0;
}

int32_t serial_dev_win32::close()
{
    bool open = false;

    is_open(open);

    if (open) {
        CloseHandle(handle);
        close_overlapped();
    }
    return 0;
}

int32_t serial_dev_win32::set_baud_rate(const uint32_t rate)
{
    BYTE* new_buf;

    INT_CHECK(await_write_completion());

    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        return GetLastError();
    }

    dcb.BaudRate = rate;

    if (!SetCommState(handle,&dcb)) {
        return GetLastError();;
    }

    return 0;

//    if (config->baudrate >= 0) {
//        for (i = 0; i < NUM_STD_BAUDRATES; i++) {
//            if (config->baudrate == std_baudrates[i].value) {
//                data->dcb.BaudRate = std_baudrates[i].index;
//                break;
//            }
//        }

//        if (i == NUM_STD_BAUDRATES)
//            data->dcb.BaudRate = config->baudrate;

//        /* Allocate write buffer for 50ms of data at baud rate. */
//        port->write_buf_size = max(config->baudrate / (8 * 20), 1);
//        new_buf = realloc(port->write_buf, port->write_buf_size);
//        if (!new_buf)
//            RETURN_ERROR(SP_ERR_MEM, "Allocating write buffer failed");
//        port->write_buf = new_buf;
//    }



//    if (config->rts >= 0) {
//        switch (config->rts) {
//        case SP_RTS_OFF:
//            data->dcb.fRtsControl = RTS_CONTROL_DISABLE;
//            break;
//        case SP_RTS_ON:
//            data->dcb.fRtsControl = RTS_CONTROL_ENABLE;
//            break;
//        case SP_RTS_FLOW_CONTROL:
//            data->dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
//            break;
//        default:
//            RETURN_ERROR(SP_ERR_ARG, "Invalid RTS setting");
//        }
//    }

//    if (config->cts >= 0) {
//        switch (config->cts) {
//        case SP_CTS_IGNORE:
//            data->dcb.fOutxCtsFlow = FALSE;
//            break;
//        case SP_CTS_FLOW_CONTROL:
//            data->dcb.fOutxCtsFlow = TRUE;
//            break;
//        default:
//            RETURN_ERROR(SP_ERR_ARG, "Invalid CTS setting");
//        }
//    }

//    if (config->dtr >= 0) {
//        switch (config->dtr) {
//        case SP_DTR_OFF:
//            data->dcb.fDtrControl = DTR_CONTROL_DISABLE;
//            break;
//        case SP_DTR_ON:
//            data->dcb.fDtrControl = DTR_CONTROL_ENABLE;
//            break;
//        case SP_DTR_FLOW_CONTROL:
//            data->dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
//            break;
//        default:
//            RETURN_ERROR(SP_ERR_ARG, "Invalid DTR setting");
//        }
//    }

//    if (config->dsr >= 0) {
//        switch (config->dsr) {
//        case SP_DSR_IGNORE:
//            data->dcb.fOutxDsrFlow = FALSE;
//            break;
//        case SP_DSR_FLOW_CONTROL:
//            data->dcb.fOutxDsrFlow = TRUE;
//            break;
//        default:
//            RETURN_ERROR(SP_ERR_ARG, "Invalid DSR setting");
//        }
//    }


//    }
}

int32_t serial_dev_win32::get_baud_rate(uint32_t &rate) const
{
    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        rate = 0;
        return GetLastError();;
    }

    rate = dcb.BaudRate;
    return 0;

//    switch (data->dcb.fRtsControl) {
//    case RTS_CONTROL_DISABLE:
//        config->rts = SP_RTS_OFF;
//        break;
//    case RTS_CONTROL_ENABLE:
//        config->rts = SP_RTS_ON;
//        break;
//    case RTS_CONTROL_HANDSHAKE:
//        config->rts = SP_RTS_FLOW_CONTROL;
//        break;
//    default:
//        config->rts = -1;
//    }

//    config->cts = data->dcb.fOutxCtsFlow ? SP_CTS_FLOW_CONTROL : SP_CTS_IGNORE;

//    switch (data->dcb.fDtrControl) {
//    case DTR_CONTROL_DISABLE:
//        config->dtr = SP_DTR_OFF;
//        break;
//    case DTR_CONTROL_ENABLE:
//        config->dtr = SP_DTR_ON;
//        break;
//    case DTR_CONTROL_HANDSHAKE:
//        config->dtr = SP_DTR_FLOW_CONTROL;
//        break;
//    default:
//        config->dtr = -1;
//    }

//    config->dsr = data->dcb.fOutxDsrFlow ? SP_DSR_FLOW_CONTROL : SP_DSR_IGNORE;
}

int32_t serial_dev_win32::set_flow_ctrl_xonoff(const ns_serial_dev::flow_ctrl_xonoff_t &fc)
{
    INT_CHECK(await_write_completion());

    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        return GetLastError();
    }

    switch (fc) {
    case flow_ctrl_xonoff_t::None :   dcb.fInX = FALSE; dcb.fOutX = FALSE; break;
    case flow_ctrl_xonoff_t::Input :  dcb.fInX = TRUE;  dcb.fOutX = FALSE; break;
    case flow_ctrl_xonoff_t::Output : dcb.fInX = FALSE; dcb.fOutX = TRUE;  break;
    case flow_ctrl_xonoff_t::Both :   dcb.fInX = TRUE;  dcb.fOutX = TRUE;  break;
    }

    if (!SetCommState(handle,&dcb)) {
        return GetLastError();;
    }

    return 0;
}

int32_t serial_dev_win32::get_flow_ctrl_xonoff(ns_serial_dev::flow_ctrl_xonoff_t &fc) const
{
    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        return GetLastError();;
    }

    fc = dcb.fInX ? (dcb.fOutX ? flow_ctrl_xonoff_t::Both   : flow_ctrl_xonoff_t::Input) :
                    (dcb.fOutX ? flow_ctrl_xonoff_t::Output : flow_ctrl_xonoff_t::None);
    return 0;
}

int32_t serial_dev_win32::set_stop_bit(const stop_bit_t &stop)
{
    INT_CHECK(await_write_completion());

    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        return GetLastError();
    }

    switch (stop) {
    case stop_bit_t::_1 :   dcb.StopBits = ONESTOPBIT;   break;
    case stop_bit_t::_1_5 : dcb.StopBits = ONE5STOPBITS; break;
    case stop_bit_t::_2 :   dcb.StopBits = TWOSTOPBITS;  break;
    }

    if (!SetCommState(handle,&dcb)) {
        return GetLastError();;
    }

    return 0;
}

int32_t serial_dev_win32::get_stop_bit(stop_bit_t &stop) const
{
    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        return GetLastError();;
    }

    switch (dcb.StopBits) {
    case ONESTOPBIT :   stop = stop_bit_t::_1;   break;
    case ONE5STOPBITS : stop = stop_bit_t::_1_5; break;
    case TWOSTOPBITS :  stop = stop_bit_t::_2;   break;
    default : return -1;
    }

    return 0;
}

int32_t serial_dev_win32::set_data_bits(const uint8_t bits)
{
    INT_CHECK(await_write_completion());

    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        return GetLastError();
    }

    dcb.ByteSize = bits;

    if (!SetCommState(handle,&dcb)) {
        return GetLastError();;
    }

    return 0;
}

int32_t serial_dev_win32::get_data_bits(uint8_t &bits) const
{
    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        bits = 8;
        return -1;
    }

    bits = dcb.ByteSize;
    return 0;
}

int32_t serial_dev_win32::set_parity_bit(const parity_bit_t &parity)
{
    INT_CHECK(await_write_completion());

    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        return GetLastError();
    }

    switch (parity) {
    case parity_bit_t::None :  dcb.Parity = NOPARITY;    break;
    case parity_bit_t::Odd :   dcb.Parity = ODDPARITY;   break;
    case parity_bit_t::Even :  dcb.Parity = EVENPARITY;  break;
    case parity_bit_t::Mark :  dcb.Parity = MARKPARITY;  break;
    case parity_bit_t::Space : dcb.Parity = SPACEPARITY; break;
    }

    if (!SetCommState(handle,&dcb)) {
        return GetLastError();;
    }

    return 0;
}

int32_t serial_dev_win32::get_parity_bit(parity_bit_t &parity) const
{
    DCB dcb;

    if (!GetCommState(handle,&dcb)) {
        return GetLastError();;
    }

    switch (dcb.Parity) {
    case NOPARITY :    parity = parity_bit_t::None;  break;
    case ODDPARITY :   parity = parity_bit_t::Odd;   break;
    case EVENPARITY :  parity = parity_bit_t::Even;  break;
    case MARKPARITY :  parity = parity_bit_t::Mark;  break;
    case SPACEPARITY : parity = parity_bit_t::Space; break;
    default : return -1;
    }

    return 0;
}

int32_t serial_dev_win32::await_write_completion()
{
    DWORD bytes_written;

    if (is_writing) {
        if (!GetOverlappedResult(handle,&write_overlapped,&bytes_written,true)) {
            return GetLastError();
        }
        is_writing = false;
    }

    return 0;
}

void serial_dev_win32::close_overlapped()
{
    CloseHandle(write_overlapped.hEvent);
    CloseHandle(read_overlapped.hEvent);
    CloseHandle(wait_overlapped.hEvent);
}
