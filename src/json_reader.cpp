#include "json_reader.h"
#include <iostream>
#include <functional>
#include <utility>



void JsonReader::accept_handler(const boost::system::error_code& error, ip::tcp::socket&& socket){
    if (!error){
        std::cout << "accepted connection!\n";
        async_read_until(socket,boost::asio::dynamic_buffer(read,128), '\n',  [this](auto const& ec, auto size) { read_handler(ec,size); });

        acceptor.async_accept(io_context, [this](auto const& ec, ip::tcp::socket socket){ accept_handler(ec, std::move(socket)); }); 
    }else{
        std::cout << "failed accept: ";
        std::cout << error.what() << "\n";
    }
}

void JsonReader::fromJSON(const nlohmann::json json){
    const std::lock_guard<std::mutex> lock(*mutex);
	try{
    	stud->setGrade(json.at("grade"));
        stud->setGpa(json.at("gpa")); 
        stud->setFirstName(json.at("fname"));
        stud->setLastName(json.at("lname"));
	}catch(std::exception ){
		stud = NULL;
	}
}

void JsonReader::read_handler(const boost::system::error_code& error, std::size_t bytes_transferred){
    if (!error){
        std::cout << "Read "<< bytes_transferred <<" bytes\n";
        fromJSON(nlohmann::json::parse(read));
        socket.close();
    }else{
        std::cout << "failed read\n";
        std::cout << error.what();
    }
}

void JsonReader::start(const unsigned short PORT){
    acceptor.async_accept(io_context, [this](auto const& ec, auto socket) { accept_handler(ec, std::move(socket)); });

    io_context.run();
}

void JsonReader::close(){
    io_context.stop();
}