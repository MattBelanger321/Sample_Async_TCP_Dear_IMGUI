#include "json_writer.h"
#include <iostream>
#include <functional>
#include <utility>


template <typename CompletionToken>
auto async_write_message(tcp::socket& socket,
    const char* message, CompletionToken&& token)
  // The return type of the initiating function is deduced from the combination
  // of CompletionToken type and the completion handler's signature. When the
  // completion token is a simple callback, the return type is void. However,
  // when the completion token is boost::asio::yield_context (used for stackful
  // coroutines) the return type would be std::size_t, and when the completion
  // token is boost::asio::use_future it would be std::future<std::size_t>.
  //
  // In C++14 we can omit the return type as it is automatically deduced from
  // the return type of our underlying asynchronous operation
{
  // When delegating to the underlying operation we must take care to perfectly
  // forward the completion token. This ensures that our operation works
  // correctly with move-only function objects as callbacks, as well as other
  // completion token types.
  return boost::asio::async_write(socket,
      boost::asio::buffer(message, std::strlen(message)),
      std::forward<CompletionToken>(token));
}

void JsonWriter::accept_handler(const boost::system::error_code& error){
    if (!error){
        std::cout << "accepted socket\n";

        boost::asio::async_write(socket, boost::asio::buffer(message.c_str(), std::strlen(message.c_str())), [this](auto const& ec, auto size) { write_handler(ec,size); });

        std::cout << "wrote: " << message << "\n";
    }else{
        std::cout << "failed accept\n";
        std::cout << error.what();
    }
}

void JsonWriter::write_handler(const boost::system::error_code& error, std::size_t bytes_transferred){
    if (!error){
        std::cout << "Message Sent: " << bytes_transferred <<"\n";
    }else{
        std::cout << "failed accept\n";
        std::cout << error.what();
    }
}

void JsonWriter::start(const unsigned short PORT){
    tcp::endpoint ep{tcp::v4(), PORT};
    acceptor.open(ep.protocol());
    acceptor.set_option(tcp::acceptor::reuse_address(true));
    acceptor.bind(ep);
    acceptor.listen();

    acceptor.async_accept(socket, [this](auto const& ec) { accept_handler(ec); });

    io_context.run();
}