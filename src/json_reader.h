#if !defined(__DEMO__JSON_READER__H__)
#define __DEMO__JSON_READER__H__

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>


using namespace boost::asio;

//Serves as the server
class JsonReader{
private:
    boost::system::error_code error;

    boost::asio::io_context io_context;
    ip::tcp::acceptor acceptor{io_context};
	ip::tcp::socket socket {io_context};

public:
    std::string read;
    void accept_handler(const boost::system::error_code& error);
    void wait_handler(const boost::system::error_code& error);
    void read_handler(const boost::system::error_code& error, std::size_t bytes_transferred);

    void start(const unsigned short PORT);
    void read_line();
    void close();
};

#endif