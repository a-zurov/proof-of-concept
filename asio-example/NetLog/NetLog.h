#pragma once


#include <memory>
#include "NetLogImpl.h"
#include "./../NetCommon/singleton.h"

namespace olc2
{

	int Add(int x, int y);

	class INetLog
	{
		std::unique_ptr< NetLogImpl> spNetLogImpl_;

	public:
		void Init() {

			spNetLogImpl_ = std::make_unique<NetLogImpl>();
		}

		void PingServer() {

			spNetLogImpl_->PingServer();
		}

		void FireLogMessage(const std::string& msg) {

			spNetLogImpl_->FireLogMessage(msg);
		}

		// FOR TESTS ONLY
		void Stop() {

			spNetLogImpl_->Stop();
		}
	};

	typedef olc::singleton< INetLog > NetLog;
}
