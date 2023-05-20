// SimpleServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ---------------------------------------------------------------------------------------------------
// Based on: David Barr, aka javidx9, OneLoneCoder 2020
// Videos:
// Part #1: https://youtu.be/2hNdkYInj4g
// Part #2: https://youtu.be/UbjxGvrDrbw


#include <iostream>
#include "../NetCommon/olc_net.h"


enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
    ProtobufTest,
    ProtobufTest2
};

class CustomServer : public olc::net::server_interface<CustomMsgTypes>
{
public:
    CustomServer(uint16_t nPort) : olc::net::server_interface<CustomMsgTypes>(nPort) {}

protected:
    bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override
    {
        DBG_DUMP();

        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerAccept;
        client->Send(msg);
        return true;
    }

    // Called when a client appears to have disconnected
    void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override
    {
        DBG_MSG_SRV("Removing client [" << client->GetID() << "]");
    }

    // Called when a message arrives
    void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client
        , olc::net::message<CustomMsgTypes>& msg) override
    {
        DBG_DUMP();

        switch (msg.header.id)
        {
        case CustomMsgTypes::ServerPing:
        {
            DBG_MSG_CNN(client->GetID(), "Server Ping");
            // Simply bounce message back to client
            client->Send(msg);
        }
        break;

        case CustomMsgTypes::MessageAll:
        {
            DBG_MSG_CNN(client->GetID(), "Message All");
            // Construct a new message and send it to all clients
            olc::net::message<CustomMsgTypes> msg;
            msg.header.id = CustomMsgTypes::ServerMessage;
            msg << client->GetID();
            MessageAllClients(msg, client);

        }
        break;

        case CustomMsgTypes::ProtobufTest:
        {
            DBG_MSG_CNN(client->GetID(), "Protobuf Test");
            // Simply bounce message back to client
            client->Send(msg);
        }
        break;

        case CustomMsgTypes::ProtobufTest2:
        {
            olc::DbgLog log;
            msg >> log;
            std::stringstream ss;
            ss << log.file() << " ( " << log.line() << " ) " << log.func();
            DBG_MSG_CNN(client->GetID(), ss.str());
        }
        break;

        }
    }
};


int main()
{
    CustomServer server(60000);
    server.Start();

    while (1)
    {
        server.Update(-1, true);
    }
}
