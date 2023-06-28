// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

NETPOPUP_API bool npdll::NetPopupInit()
{
    return NetPopup::instance().Init();
}

NETPOPUP_API void npdll::NetPopupPing()
{
    return NetPopup::instance().Ping();
}

NETPOPUP_API void npdll::NetPopupFree()
{
    NetPopup::instance().Stop();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

