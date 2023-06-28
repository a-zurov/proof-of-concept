#pragma once

#include <memory>
#include "./../NetCommon/singleton.h"
#include "NetPopupImpl.h"


#ifdef NETPOPUP_EXPORTS
#define NETPOPUP_API __declspec(dllexport)
#else
#define NETPOPUP_API __declspec(dllimport)
#endif

namespace npdll
{
    class INetPopup
    {
        std::unique_ptr<NetPopupImpl> spNetPopupImpl_;

    public:
        bool Init() {

            spNetPopupImpl_ = std::make_unique<NetPopupImpl>();
            return spNetPopupImpl_->Init();
        }

        void Ping() {

            spNetPopupImpl_->PingServer();
        }

        void Stop() {

            spNetPopupImpl_->Stop();
        }
    };

    typedef olc::singleton< INetPopup > NetPopup;

    NETPOPUP_API bool NetPopupInit();

    NETPOPUP_API void NetPopupPing();

    NETPOPUP_API void NetPopupFree();
}
