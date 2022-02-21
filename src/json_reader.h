#if !defined(__DEMO__JSON_READER__H__)
#define __DEMO__JSON_READER__H__

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>


using namespace boost::asio;

class JsonReader{
private:
    io_context io_context;
    boost::system::error_code error;
    //create socket
    ip::tcp::socket socket{io_context};

public:
    std::string read;
    void connect_handler(const boost::system::error_code& error);
    void read_handler(const boost::system::error_code& error, std::size_t bytes_transferred);

    void start(const std::string IPADDR, const unsigned short PORT);
    void close();
};

#endif