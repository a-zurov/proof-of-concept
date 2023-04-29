// asio_example.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------------------------------------------
// https://www.youtube.com/watch?v=2hNdkYInj4g

#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>


int main() {

    boost::system::error_code ec;

    boost::asio::io_context context; // the platform specific interface

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("93.184.216.34"), 80);

    boost::asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if(!ec)
        std::cout << "Connection success!\n";
    else
        std::cout << "Connection failed: " << ec.message() << '\n';
}
