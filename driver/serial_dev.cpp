#include "serial_dev.h"
//#include <boost/asio.hpp>
//#include <boost/bind/bind.hpp>

#ifdef PLATFORM_WIN32
    #define IMP serial_dev_win32
#else
#endif

using namespace std;
//using namespace boost;
//using namespace boost::asio;
//using namespace boost::placeholders;

#define PORT ((serial_port *)(_port))
#define IO_SERVICE ((io_service *)(_io_service))

serial_dev::serial_dev()
{

}

serial_dev::~serial_dev()
{

}

int32_t serial_dev::get_devs(std::vector<std::string> &devs)
{
    return 0;
}

int32_t serial_dev::open(const std::string &dev)
{
    return 0;
}

int32_t serial_dev::is_open(bool &open) const
{
    return 0;
}

int32_t serial_dev::close()
{
    return 0;
}

int32_t serial_dev::set_baud_rate(const uint32_t rate)
{
    return 0;
}

int32_t serial_dev::get_baud_rate(uint32_t &rate) const
{
    return 0;
}

int32_t serial_dev::set_flow_ctrl(const serial_dev_flow_ctrl_t &fc)
{
    return 0;
}

int32_t serial_dev::get_flow_ctrl(serial_dev_flow_ctrl_t &fc) const
{
    return 0;
}

int32_t serial_dev::set_parity_bit(const serial_dev_parity_bit_t &parity)
{
    return 0;
}

int32_t serial_dev::get_parity_bit(serial_dev_parity_bit_t &parity) const
{
    return 0;
}

int32_t serial_dev::set_stop_bit(const serial_dev_stop_bit_t &stop)
{
    return 0;
}

int32_t serial_dev::get_stop_bit(serial_dev_stop_bit_t &stop) const
{
    return 0;
}

int32_t serial_dev::set_data_bits(const uint8_t bits)
{
    return 0;
}

int32_t serial_dev::get_data_bits(uint8_t &bits) const
{
    return 0;
}

int32_t serial_dev::set_timeout(const uint32_t timeout)
{
    _timeout = timeout;
    _timeout = _timeout > 0 ? _timeout : UINT32_MAX;
    return 0;
}

int32_t serial_dev::get_timeout(uint32_t &timeout) const
{
    timeout = _timeout;
    return 0;
}

int32_t serial_dev::set_terminate_char_en(const bool en)
{
    return 0;
}

int32_t serial_dev::get_terminate_char_en(bool &en) const
{
    return 0;
}

int32_t serial_dev::set_terminate_char(const char str)
{
    return 0;
}

int32_t serial_dev::get_terminate_char(char &str) const
{
    return 0;
}

int32_t serial_dev::write(const string &data) const
{
    return 0;
}

//void handle_read(string &buf,boost::system::error_code ec,std::size_t bytes_transferred)
//{
//    ignore_unused(buf,ec,bytes_transferred);
//}

int32_t serial_dev::read()
{
    return 0;
}

int32_t serial_dev::read(string &buf)
{
    return 0;
}

int32_t serial_dev::set_read_end_mode(const serial_dev_read_end_mode_t &method)
{
    return 0;
}

int32_t serial_dev::get_read_end_mode(serial_dev_read_end_mode_t &method) const
{
    return 0;
}

bool serial_dev::is_open() const
{
    bool open = true;
    is_open(open);
    return open;
}

int32_t serial_dev::read(string &buf,const uint32_t timeout)
{
    set_timeout(timeout);

    return read(buf);
}

int32_t serial_dev::read_until(const char str)
{
    string std_str;

    std_str.push_back(str);
    return read_until(std_str);
}

int32_t serial_dev::read_until(const char str,const uint32_t timeout)
{
    INT_CHECK(set_timeout(timeout));
    return read_until(str);
}

int32_t serial_dev::read_until(const string &str)
{
    string buf;

    while (1) {
        INT_CHECK(read(buf));
        if (buf.find(str) != string::npos) {
            return 0;
        }
    }
}

int32_t serial_dev::read_until(const string &str,const uint32_t timeout)
{
    INT_CHECK(set_timeout(timeout));
    return read_until(str);
}

int32_t serial_dev::read_until(const std::regex &reg)
{
    string buf;

    while (1) {
        INT_CHECK(read(buf));
        if (regex_match(buf,reg)) {
            return 0;
        }
    }
}

int32_t serial_dev::read_until(const std::regex &reg,const uint32_t timeout)
{
    INT_CHECK(set_timeout(timeout));
    return read_until(reg);
}
