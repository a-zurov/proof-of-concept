
#include <windows.h>
#include <filesystem>

#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void main(array<String^>^ args) {

    wchar_t wszFullPath[MAX_PATH] = {};
    GetCurrentDirectory(MAX_PATH, wszFullPath);
    std::filesystem::path path{ wszFullPath };
    path.concat(L"\\..\\Debug\\NetPopup.dll");

    HMODULE hLib = LoadLibrary(path.c_str());

    if (hLib)
    {
        typedef bool(__stdcall* NetPopupInit)(void);
        NetPopupInit pfarInit = (NetPopupInit)GetProcAddress(hLib, "NetPopupInit");

        typedef void(__stdcall* NetPopupFree)(void);
        NetPopupFree pfarFree = (NetPopupFree)GetProcAddress(hLib, "NetPopupFree");

        if (pfarInit)
        {
            if (pfarInit())
            {
                Application::EnableVisualStyles();
                Application::SetCompatibleTextRenderingDefault(false);

                WinFormCpp::MyForm form;

                form.SetFarProcPing(GetProcAddress(hLib, "NetPopupPing"));

                Application::Run(% form);
            }

            pfarFree();
        }

        FreeLibrary(hLib);
    }
}