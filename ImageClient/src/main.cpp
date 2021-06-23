#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;
using boost::asio::io_context;
int main(int arg, char *argv[])
{
    if(arg != 2){
        std::cerr << "Usage: client <host>" << std::endl;
        return 1;
    }      
    io_context io;
    tcp::resolver ressolver(io);
    boost::asio::ip::address addr;
    boost::system::error_code ec1;
    addr = boost::asio::ip::address::from_string("127.0.0.1", ec1);
    tcp::endpoint endpoint(addr, 3456);

    tcp::socket socket(io);
    socket.connect(endpoint);

    for(; ;){
        boost::array<char, 128> buf;
        boost::system::error_code ec;

        size_t len = socket.read_some(boost::asio::buffer(buf), ec);

        if(ec == boost::asio::error::eof)
            break;
        std::cout.write(buf.data(), len);
    }

    return 0;
}






