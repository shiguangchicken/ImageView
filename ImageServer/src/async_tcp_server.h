#ifndef _AsyncTcpServer_H
#define _AsyncTcpServer_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

using boost::asio::io_context;
using boost::asio::ip::tcp;
typedef boost::shared_ptr<tcp::socket> socket_ptr;
class AsyncTcpServer
{
private:
    /* data */
    tcp::acceptor   *_acceptor;
    io_context      *_io_context;

public:
    AsyncTcpServer(io_context *io);
    ~AsyncTcpServer();

    void start_accept();

private:

    std::string make_time_string();

    void handle_connection(socket_ptr socket, const boost::system::error_code &err);

    int write_time(socket_ptr socket);

    void handle_write(boost::system::error_code err, size_t len);


};



#endif