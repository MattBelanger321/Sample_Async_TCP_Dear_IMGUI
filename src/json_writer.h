#if !defined(__DEMO__JSON_WRITER__H__)
#define __DEMO__JSON_WRITER__H__

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>


using boost::asio::ip::tcp;


class JsonWriter{
private:
	boost::asio::io_context io_context;
    tcp::acceptor acceptor{io_context};
	tcp::socket socket {io_context};

public:
    std::string message;
    void accept_handler(const boost::system::error_code& error);
    void write_handler(const boost::system::error_code& error, std::size_t bytes_transferred);
    void start(const unsigned short PORT);
    void close();
};

#endif