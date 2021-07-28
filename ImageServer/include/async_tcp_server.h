#ifndef _AsyncTcpServer_H
#define _AsyncTcpServer_H

#include <boost/asio.hpp>
#include <list>
#include <string>

using boost::asio::io_context;
using boost::asio::ip::tcp;
using boost::system::error_code;

class Session 
{
    const int MAX_BUFFER_LEN = 1024*1024*20; //20M
    struct BuffRW_t
    {
        char* pbuf;
        int len;
    };
    
private:
    tcp::socket m_socket;
    BuffRW_t m_readBuffer;
    BuffRW_t m_writeBuffer;
public:
    Session(tcp::socket sock);
    ~Session();
    
    void Start(); 

    void Stop();

    void Ping();

    bool GetPing(); 
private:
    void ReadFromClient();

    void WriteToClient();       
};

class AsyncTcpServer
{
private:
    /* data */
    tcp::acceptor   m_acceptor;
    io_context&      m_ioContext;
    std::list<Session*> m_sessions;   // all client sessions
    boost::asio::steady_timer m_timer; // check client is alive
    bool m_isPinged;
public:
    AsyncTcpServer(io_context& io, u_int16_t port);
    ~AsyncTcpServer();

    void StartAccept();

private:

    void Check();


};



#endif