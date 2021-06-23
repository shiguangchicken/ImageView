#include <iostream>
#include <string>
#include "async_tcp_server.h"

int main()
{
    try{
        io_context io;
        AsyncTcpServer *tcp_server = new AsyncTcpServer(&io);
        tcp_server->start_accept();
        io.run();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}