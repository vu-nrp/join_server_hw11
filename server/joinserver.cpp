#include "joinserver.h"
#include <boost/asio/ip/tcp.hpp>

//!
//! Connection
//!

Connection::Connection(bi::tcp::socket socket, db::Interpreter &interpreter) :
    m_socket(std::move(socket)),
    m_interpreter(interpreter)
{
}

void Connection::run()
{
    resetBuffer();
    readData();
}

void Connection::resetBuffer()
{
    std::fill(m_buffer, m_buffer + Connection::BufferLength, 0);
}

void Connection::readData()
{
    const auto self = shared_from_this();
    m_socket.async_read_some(ba::buffer(m_buffer, Connection::BufferLength), [this, self](boost::system::error_code errCode, size_t /*size*/)
    {
        if (!errCode) {
            const auto response = m_interpreter.process(m_buffer);
            resetBuffer();
            std::copy(response.cbegin(), response.cend(), m_buffer);
            writeData(response.size());
        }
    });
}

void Connection::writeData(const size_t length)
{
    const auto self = shared_from_this();
    ba::async_write(m_socket, ba::buffer(m_buffer, length), [this, self](boost::system::error_code errCode, size_t /*size*/)
    {
        if (!errCode)
            run();
    });
}

//!
//! JoinServer
//!

JoinServer::JoinServer(ba::io_context &context, const bi::tcp::endpoint &point) :
    m_acceptor(context, point),
    m_socket(context)
{
    accept();
}

void JoinServer::accept()
{
    m_acceptor.async_accept(m_socket, [this](boost::system::error_code errCode)
    {
        if (!errCode)
            std::make_shared<Connection>(std::move(m_socket), m_interpreter)->run();

        accept();
    });
}
