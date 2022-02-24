#include "json_reader.h"
#include <iostream>
#include <functional>
#include <utility>



void JsonReader::accept_handler(const boost::system::error_code& error, JsonReader& jr){
    if (!error){
        std::cout << "accepted connection!\n";
        async_read_until(socket,boost::asio::dynamic_buffer(read,128), '\n',  [this, &jr](auto const& ec, auto size) { read_handler(ec,size, jr); });

        JsonReader new_jr(message,mutex,io_context);
        new_jr.start(55555);
        new_jr.acceptor.async_accept(new_jr.socket, [this, &new_jr](auto const& ec){ accept_handler(ec, new_jr); }); 
    }else{
        std::cout << "failed accept: ";
        std::cout << error.what() << "\n";
        std::exit(0);
    }
}

void JsonReader::saveStudent(const Student* stud){
	const std::lock_guard<std::mutex> lock(*mutex);
	*message = stud->getFirstName() + ", " + stud->getLastName() + ", " + std::to_string(stud->getGrade()) + ", " + std::to_string(stud->getGpa());
}

Student JsonReader::fromJSON(const nlohmann::json json){
	Student stud;
	try{
    	stud =  {json.at("grade"), json.at("gpa"), json.at("fname"), json.at("lname")};
	}catch(std::exception e){
		stud = NULL;
	}
    return stud;
}

void JsonReader::read_handler(const boost::system::error_code& error, std::size_t bytes_transferred, JsonReader& jr){
    if (!error){
        std::cout << "Read "<< bytes_transferred <<" bytes\n";
        Student stud = fromJSON(nlohmann::json::parse(read));
	    saveStudent(&stud);
        jr.socket.close();
        jr.acceptor.close();
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


    acceptor.async_accept(socket, [this](auto const& ec) { accept_handler(ec,*this); });

    io_context.run();
}

void JsonReader::close(){
    io_context.stop();
}