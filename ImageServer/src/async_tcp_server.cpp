#include <ctime>
#include <iostream>
#include <boost/bind/bind.hpp>
#include "async_tcp_server.h"

AsyncTcpServer::AsyncTcpServer(io_context *io)
{
    _io_context = io;
    tcp::endpoint ep(tcp::v4(), 3456);
    _acceptor   = new tcp::acceptor(*_io_context, ep);
}

AsyncTcpServer::~AsyncTcpServer()
{
    if(_acceptor){
        delete _acceptor;
    }
}
std::string AsyncTcpServer::make_time_string()
{
    time_t now = time(0);
    return ctime(&now);
}

void AsyncTcpServer::start_accept()
{
    socket_ptr  _socket( new tcp::socket(*_io_context) );
    _acceptor->async_accept(*_socket, boost::bind(&AsyncTcpServer::handle_connection,this,
            _socket, boost::asio::placeholders::error));
}

void AsyncTcpServer::handle_connection(socket_ptr socket, const boost::system::error_code &err)
{
    if(!err){
        write_time(socket);
    }
    start_accept();
}

int AsyncTcpServer::write_time(socket_ptr socket)
{
    std::string str = make_time_string();
    socket->async_write_some(boost::asio::buffer(str), 
        boost::bind(&AsyncTcpServer::handle_write, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void AsyncTcpServer::handle_write(boost::system::error_code err, size_t len)
{
    if(len > 0){
            std::cout << "process client ok" << std::endl;
        }else{
            std::cout << "failed to wirte client" << std::endl;
        }
}