#pragma once


#include <iostream>
#include <sstream>
#include <thread>

#include "../NetCommon/olc_net.h"

namespace olc2
{
	enum class CustomMsgTypes : uint32_t
	{
		ServerAccept,
		ServerDeny,
		ServerPing,
		MessageAll,
		ServerMessage,
		ProtobufTest,

		Stop = 1000
	};

	class NetLogImpl : public olc::net::client_interface<CustomMsgTypes>
	{
		std::thread th_;

	public:
		~NetLogImpl() {
			Stop();
		}

		NetLogImpl() : th_(
			[this]() {
				for (;;) {
					if (IsConnected())
					{
						auto msg = Incoming().pop_n_wait_front().msg;

						switch (msg.header.id)
						{
						case CustomMsgTypes::Stop:
						{
							// Server has responded to a ping request
							DBG_MSG_CLT("Client stopped!");
							return;
						}

						case CustomMsgTypes::ServerAccept:
						{
							// Server has responded to a ping request
							DBG_MSG_CLT("Server Accepted Connection");
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
			})
		{
			Connect("127.0.0.1", 60000);
		}

			void PingServer()
			{
				olc::net::message<CustomMsgTypes> msg;
				msg.header.id = CustomMsgTypes::ServerPing;
				// Caution with this...
				std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
				msg << timeNow;
				Send(msg);
			}

			void FireLogMessage(const std::string& msg) {

				std::cout << msg << '\n';
			}

			// private:
			void Stop()
			{

				olc::net::owned_message<CustomMsgTypes> own_msg;
				olc::net::message<CustomMsgTypes> msg;
				msg.header.id = CustomMsgTypes::Stop;
				own_msg.msg = msg;

				Incoming().push_back_notify(own_msg);

				if (th_.joinable()) {
					try {
						th_.join();
					}
					catch (std::system_error& ex) {
						std::cout << ex.what();
					}
				}
			}

	};
}
