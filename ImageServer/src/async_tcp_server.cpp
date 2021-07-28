#include <ctime>
#include <iostream>
#include <boost/bind/bind.hpp>
#include "async_tcp_server.h"

using boost::asio::buffer;

Session::Session(tcp::socket sock)
: m_socket(std::move(sock))
{
    m_readBuffer.pbuf = new char[MAX_BUFFER_LEN];
    m_writeBuffer.pbuf = new char[MAX_BUFFER_LEN];
    m_readBuffer.len = 0;
    m_writeBuffer.len = 0;
}

Session::~Session()
{
    delete m_readBuffer.pbuf;
    delete m_writeBuffer.pbuf;
}

void Session::Start()
{
    ReadFromClient();
}

void Session::Stop()
{
    m_socket.close();
}

void Session::Ping()
{
    //todo fill ping buffer
    WriteToClient();
}

bool Session::GetPing()
{
    return true;
}

void Session::ReadFromClient()
{
    m_socket.async_read_some(buffer(m_readBuffer.pbuf, MAX_BUFFER_LEN),
        [this](error_code err, size_t len)
        {
            if(!err)
            {
                m_readBuffer.len = len;
                //todo    deal the buffer

                ReadFromClient();
            }
            else
            {
                //todo  deal error
            }
        });
}

void Session::WriteToClient()
{
    m_socket.async_write_some(buffer(m_writeBuffer.pbuf, m_writeBuffer.len),
        [this](error_code err, size_t len)
        {
            if(!err)
            {
                //todo    deal the buffer
            }
            else
            {
                //todo  deal error
            }
        });
}

AsyncTcpServer::AsyncTcpServer(io_context& io, u_int16_t port)
: m_ioContext(io),m_acceptor(io),m_timer(io),m_isPinged(false)
{
    tcp::endpoint ep(tcp::v4(), port);
    m_acceptor = std::move(tcp::acceptor(m_ioContext, ep));
    m_timer = std::move(boost::asio::steady_timer(m_ioContext, boost::asio::chrono::seconds(60)));
    m_timer.async_wait(boost::bind(&AsyncTcpServer::Check, this));
}

AsyncTcpServer::~AsyncTcpServer()
{
    while (!m_sessions.empty())
    {
        Session* p = m_sessions.front();
        p->Stop();
        delete p;
        m_sessions.pop_front();
    }
    
}


void AsyncTcpServer::StartAccept()
{
    m_acceptor.async_accept(
        [this](error_code ec, tcp::socket sock)
        {
            if(!ec)
            {
                Session* p = new Session(std::move(sock));
                m_sessions.push_back(p);
                p->Start();
                std::cout << "recv a client \n";
            }
            StartAccept();
        });
}

void AsyncTcpServer::Check()
{

    if(!m_isPinged) //ping
    {
        for (auto &&i : m_sessions)
            i->Ping();
    }
    else        //get ping result
    {
        for (auto &&i : m_sessions)
        {
            if(!i->GetPing())
            {
                i->Stop();
                m_sessions.remove(i);
                delete i;
            }                    
        }
    }
    m_timer.async_wait(boost::bind(&AsyncTcpServer::Check, this));     

}