#include <iostream>
#include <boost/asio.hpp>
#include "server/joinserver.h"

//!
//! \brief main - app entry point
//!

int main(int argc, char *argv[])
{
//    std::cout << "Home work #11" << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: join_server <port>" << std::endl;
        return 1;
    }

    try {
        ba::io_context context;
        JoinServer server(context, bi::tcp::endpoint(bi::tcp::v4(), std::atoi(argv[1])));

        context.run();
    } catch (std::exception &exc) {
        std::cout << exc.what() << std::endl;
    }

    return 0;
}
