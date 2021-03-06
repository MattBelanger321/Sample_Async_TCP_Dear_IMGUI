#if !defined(__DEMO__JSON_READER__H__)
#define __DEMO__JSON_READER__H__

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/write.hpp>

#include "student.h"
#include <nlohmann/json.hpp>

using namespace boost::asio;

//Serves as the server
class JsonReader{
private:
    const unsigned short PORT;
    boost::system::error_code error;
    boost::asio::io_context io_context;
    ip::tcp::acceptor acceptor;
	ip::tcp::socket socket;

    std::mutex* mutex;  //pointer to UI message mutex
    Student *stud;

public:
    std::string read;

    //important:: io_context is passed by reference so all instanced of JsonReader have the same io_context
    JsonReader(std::mutex *mutex, Student *stud, const unsigned short port): PORT(port), acceptor(io_context), socket(io_context){
        this->mutex = mutex;
        this->stud = stud;

        ip::tcp::endpoint ep{ip::tcp::v4(), PORT};
        acceptor.open(ep.protocol());
        acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
        acceptor.bind(ep);
        acceptor.listen();
    }

    void accept_handler(const boost::system::error_code& error, ip::tcp::socket&& socket);
    void read_handler(const boost::system::error_code& error, std::size_t bytes_transferred);

    void start(const unsigned short PORT);
    void close();

    void fromJSON(const nlohmann::json json);
};

#endif