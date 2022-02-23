#include "json_writer.h"
#include <iostream>
#include <functional>
#include <utility>


template <typename CompletionToken>
auto async_write_message(ip::tcp::socket& socket,const char* message, CompletionToken&& token)
{

  return boost::asio::async_write(socket,
      boost::asio::buffer(message, std::strlen(message)),
      std::forward<CompletionToken>(token));
}

void JsonWriter::connect_handler(const boost::system::error_code& error){
    if (!error){
        std::cout << "Connected!\n";

        boost::asio::async_write(socket, boost::asio::buffer(message.c_str(), std::strlen(message.c_str())), [this](auto const& ec, auto size) { write_handler(ec,size); });

        std::cout << "wrote: " << message << "\n";
    }else{
        std::cout << "failed to connect\n";
        std::cout << error.what();
    }
}

void JsonWriter::write_handler(const boost::system::error_code& error, std::size_t bytes_transferred){
    if (!error){
        std::cout << "Message Sent: " << bytes_transferred <<"\n";
    }else{
        std::cout << "failed to write\n";
        std::cout << error.what();
    }
}

void JsonWriter::close(){
    socket.close();
}

void JsonWriter::start(const std::string IPADDR, const unsigned short PORT){
    //connect to server
    socket.async_connect(ip::tcp::endpoint(ip::make_address(IPADDR.c_str()),PORT), [this](auto const& ec) { connect_handler(ec); });

    io_context.run();
}
