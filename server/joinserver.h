#pragma once

#include <boost/asio.hpp>
#include "processing.h"

//!
//! namespace name reducing
//!

namespace ba = boost::asio;
namespace bi = ba::ip;

//!
//! \brief The Connection class - connection class
//!

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    Connection(bi::tcp::socket socket, db::Interpreter &interpreter);

    void run();

private:
    static const auto BufferLength {1000};

    void resetBuffer();
    void readData();
    void writeData(const size_t length);

    bi::tcp::socket m_socket;
    char m_buffer[BufferLength];
    db::Interpreter &m_interpreter;

};

//!
//! \brief The JoinServer class - main server class
//!

class JoinServer
{
public:
    JoinServer(ba::io_context &context, const bi::tcp::endpoint &point);

private:
    void accept();

    bi::tcp::acceptor m_acceptor;
    bi::tcp::socket m_socket;
    db::Interpreter m_interpreter;

};
