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
    boost::system::error_code error;
    boost::asio::io_context &io_context;
    ip::tcp::acceptor acceptor;
	ip::tcp::socket socket;

    std::string* message; //pointer to UI text
    std::mutex* mutex;  //pointer to UI message mutex

public:
    std::string read;

    //important:: io_context is passed by reference so all instanced of JsonReader have the same io_context
    JsonReader(std::string *message, std::mutex *mutex, boost::asio::io_context& io): io_context(io),acceptor(io), socket(io){
        this->message = message;
        this->mutex = mutex;
    }

    void accept_handler(const boost::system::error_code& error, JsonReader& jr);
    void read_handler(const boost::system::error_code& error, std::size_t bytes_transferred, JsonReader& jr);

    void start(const unsigned short PORT);
    void close();

    void saveStudent(const Student* stud);
    Student fromJSON(const nlohmann::json json);
};

#endif