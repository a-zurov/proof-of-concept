// SimpleClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ---------------------------------------------------------------------------------------------------
// Based on: David Barr, aka javidx9, OneLoneCoder 2020
// Videos:
// Part #1: https://youtu.be/2hNdkYInj4g
// Part #2: https://youtu.be/UbjxGvrDrbw


#ifdef _WIN32
#include <conio.h>
#else
#include "linux_helper.h"
#endif

#include <iostream>
#include "../NetCommon/olc_net.h"


enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
    ProtobufTest
};

class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{
public:
    void PingServer()
    {
        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerPing;

        // Caution with this...
        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        msg << timeNow;
        Send(msg);
    }

    void MessageAll()
    {
        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::MessageAll;
        Send(msg);
    }

    void ProtobufTest()
    {
        olc::Person person;
        person.set_name("Alice");
        person.set_age(42);
        person.add_phone_number("+7905905905");
        person.add_phone_number("+1-399-399-399");

        olc::Address address;
        address.set_building(16);
        address.set_street("Lva");
        *(address.mutable_street()) += " Tolstogo";

        *(person.mutable_address()) = address;
        *(person.mutable_name()) += " Cooper";

        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ProtobufTest;
        msg << person;
        Send(msg);
    }
};


int main()
{
    //using namespace std::chrono_literals;
    //std::this_thread::sleep_for(10000ms);

    CustomClient c;
    c.Connect("127.0.0.1", 60000);

    char ch;
    bool key[3] = { false, false, false };
    bool old_key[3] = { false, false, false };
    bool bQuit = false;

    //setvbuf(stdout, NULL, _IONBF, 0); // Set stdout unbuffered
    std::cout << "press: 1 = ping, 2 = msg all, 3 = proto test, 4 = quit ..\n";

    while (!bQuit)
    {
//#define ASYNC_KEY_ <- NOT ALLOWED NOW
#ifdef ASYNC_KEY_

#ifdef _WIN32

        if (GetForegroundWindow() == GetConsoleWindow())
        {
            key[0] = GetAsyncKeyState('1') & 0x8000;
            key[1] = GetAsyncKeyState('2') & 0x8000;
            key[2] = GetAsyncKeyState('3') & 0x8000;
        }

        if (key[0] && !old_key[0]) c.PingServer();
        if (key[1] && !old_key[1]) c.MessageAll();
        if (key[2] && !old_key[2]) bQuit = true;

        for (int i = 0; i < 3; i++) old_key[i] = key[i];

#else // NOT _WIN32

/*
        int key = keycode_of_key_being_pressed();
        if (key > 0) {
            std::cout << key << '\n';
            if (10 == key)
                c.PingServer();
            if (11 == key)
                c.MessageAll();
            else if (12 == key)
                bQuit = true;
        }
*/

#endif // NOT _WIN32

#else // NOT ASYNC_KEY_

        // program pauses here until key is pressed
        ch = _getch();
        if ('1' == ch)
            c.PingServer();
        if ('2' == ch)
            c.MessageAll();
        if ('3' == ch)
            c.ProtobufTest();
        else if ('4' == ch)
            bQuit = true;


#endif // ASYNC_KEY_

        if (c.IsConnected())
        {
            if (!c.Incoming().empty())
            {
                auto msg = c.Incoming().pop_front().msg;

                switch (msg.header.id)
                {
                case CustomMsgTypes::ServerAccept:
                {
                    // Server has responded to a ping request
                    std::cout << "Server Accepted Connection\n";
                }
                break;

                case CustomMsgTypes::ServerPing:
                {
                    // Server has responded to a ping request
                    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                    std::chrono::system_clock::time_point timeThen;
                    msg >> timeThen;
                    DBG_MSG_CLT("Ping: " << std::chrono::duration<double>(timeNow - timeThen).count());
                }
                break;

                case CustomMsgTypes::ServerMessage:
                {
                    // Server has responded to a ping request
                    uint32_t clientID;
                    msg >> clientID;
                    DBG_MSG_CLT("Hello from [" << clientID << "]");
                }
                break;


                case CustomMsgTypes::ProtobufTest:
                {
                    olc::Person person;
                    msg >> person;
                    DBG_MSG_CLT("Name: " << person.name() << " Street: " << person.address().street());
                }
                break;

                }
            }
        }
        else
        {
            DBG_MSG_CLT("Shut Down");
            bQuit = true;
        }
    }
}
