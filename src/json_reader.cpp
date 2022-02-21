#include "json_reader.h"
#include <iostream>
#include <functional>
#include <utility>


void JsonReader::connect_handler(const boost::system::error_code& error){
    if (!error){
        std::cout << "accepted socket\n";

        //read from server
        
        async_read_until(socket,boost::asio::dynamic_buffer(read,128), '\n',  [this](auto const& ec, auto size) { read_handler(ec,size); });

    }else{
        std::cout << "failed accept: ";
        std::cout << error.what() << "\n";
    }
}

void JsonReader::read_handler(const boost::system::error_code& error, std::size_t bytes_transferred){
    if (!error){
        std::cout << "accepted recieve "<< bytes_transferred <<"\n";
    }else{
        std::cout << "failed read\n";
        std::cout << error.what();
    }
}

void JsonReader::start(const std::string IPADDR, const unsigned short PORT){
    //connect to server
    socket.async_connect(ip::tcp::endpoint(ip::make_address(IPADDR.c_str()),PORT), [this](auto const& ec) { connect_handler(ec); });

    io_context.run();
}

void JsonReader::close(){
    socket.close();
}