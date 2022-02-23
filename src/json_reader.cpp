#include "json_reader.h"
#include <iostream>
#include <functional>
#include <utility>

void JsonReader::read_line(){
    //read data from client when available
    socket.async_wait(boost::asio::ip::tcp::socket::wait_read,  [this](auto const& ec) { wait_handler(ec); });

    io_context.run();
}

void JsonReader::accept_handler(const boost::system::error_code& error){
    if (!error){
        std::cout << "accepted connection!\n";
    }else{
        std::cout << "failed accept: ";
        std::cout << error.what() << "\n";
    }
}

void JsonReader::wait_handler(const boost::system::error_code& error){
    if(!error){
        std::cout << "Wait Complete!\n";
        async_read_until(socket,boost::asio::dynamic_buffer(read,128), '\n',  [this](auto const& ec, auto size) { read_handler(ec,size); });
    }else{
        std::cout << "failed accept: ";
        std::cout << error.what() << "\n";
    }
}

void JsonReader::read_handler(const boost::system::error_code& error, std::size_t bytes_transferred){
    if (!error){
        std::cout << "Read "<< bytes_transferred <<" bytes\n";
    }else{
        std::cout << "failed read\n";
        std::cout << error.what();
    }
}

void JsonReader::start(const unsigned short PORT){

    ip::tcp::endpoint ep{ip::tcp::v4(), PORT};
    acceptor.open(ep.protocol());
    acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor.bind(ep);
    acceptor.listen();

    acceptor.async_accept(socket, [this](auto const& ec) { accept_handler(ec); });

    io_context.run();
}

void JsonReader::close(){
    socket.close();
}