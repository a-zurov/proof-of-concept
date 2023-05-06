// asio_example.cpp : This file contains the 'main' function. Program execution begins and ends there.
// --------------------------------------------------------------------------------------------------
// https://www.youtube.com/watch?v=2hNdkYInj4g

#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#define ASYNC_READ_ // <- async vs sync read switch
#ifdef ASYNC_READ_
#define SIZE_OF_CHUNK 1024

std::vector<char> g_vecBuffer(SIZE_OF_CHUNK);

void GrabSomeData(boost::asio::ip::tcp::socket& socket) {

    socket.async_read_some( boost::asio::buffer(g_vecBuffer.data(), g_vecBuffer.size())
                          , [&socket](boost::system::error_code ec, size_t length) {

                                    std::cout << "\n\n" << "Async_read_some: " << length << " bytes\n\n";

                                    if (!ec) {
                                        for (size_t j = 0; j < length; ++j)
                                            std::cout << g_vecBuffer[j];
                                        GrabSomeData(socket);
                                    }
                                    else {
                                        std::cout << "Async_read_some failed: " << ec.message() << '\n';
                                    }
                                }
                          );
}
#endif // ASYNC_READ_


int main() {

    boost::system::error_code ec;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("93.184.216.34"), 80);

    boost::asio::io_context context; // the platform specific interface

#ifdef ASYNC_READ_

    boost::asio::io_context::work idle(context);

    std::thread context_thread{ [&context]() { context.run(); } };

#endif // ASYNC_READ_

    boost::asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if(!ec)
        std::cout << "Connection success!\n";
    else
        std::cout << "Connection failed: " << ec.message() << '\n';

    if (socket.is_open()) {

#ifdef ASYNC_READ_
        GrabSomeData(socket);
#endif

        std::string szRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(boost::asio::buffer(szRequest.data(), szRequest.size()), ec);

#ifndef ASYNC_READ_

        socket.wait(socket.wait_read);

        size_t bytes = socket.available();
        std::cout << "Socket has availiable " << bytes << "bytes\n";

        if (bytes > 0) {
            std::vector<char> vecBuffer(bytes);
            socket.read_some(boost::asio::buffer(vecBuffer.data(), vecBuffer.size()), ec);

            for (auto ch : vecBuffer) {
                std::cout << ch;
            }
            std::cout << std::endl;
        }
#endif // NOT ASYNC_READ_

    }

#ifdef ASYNC_READ_
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(400ms);
    context.stop();

    if (context_thread.joinable()) {
        try {
            context_thread.join();
        }
        catch (std::system_error& ex) {
            std::cout << ex.what();
        }
    }
#endif // ASYNC_READ_
}
