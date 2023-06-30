
#include <windows.h>
#include <filesystem>

#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

delegate void MyCallback(String^ str);

void OnPingResponse(const std::string& s) {

    MyCallback^ callback = gcnew MyCallback(&WinFormCpp::MyForm::OnPingResponse);
    System::String^ s1 = gcnew String(s.c_str());
    callback(s1);
}


[STAThreadAttribute]
void main(array<String^>^ args) {

    wchar_t wszFullPath[MAX_PATH] = {};
    GetCurrentDirectory(MAX_PATH, wszFullPath);
    std::filesystem::path path{ wszFullPath };
    path.concat(L"\\..\\Debug\\NetPopup.dll");

    HMODULE hLib = LoadLibrary(path.c_str());

    if (hLib)
    {
        typedef bool(__stdcall* FARProcInit)(void);
        FARProcInit pfarInit = (FARProcInit)GetProcAddress(hLib, "NetPopupInit");

        typedef void(__stdcall* FARProcBindPingCallback)(void*);
        FARProcBindPingCallback pfarBindPingCallback =
            (FARProcBindPingCallback)GetProcAddress(hLib, "NetPopupBindPingCallback");

        typedef void(__stdcall* FARProcFree)(void);
        FARProcFree pfarFree = (FARProcFree)GetProcAddress(hLib, "NetPopupFree");

        if (pfarInit)
        {
            if (pfarInit())
            {
                Application::EnableVisualStyles();
                Application::SetCompatibleTextRenderingDefault(false);

                WinFormCpp::MyForm form;

                pfarBindPingCallback(&OnPingResponse);

                form.SetFARProcPing(GetProcAddress(hLib, "NetPopupPing"));

                Application::Run(% form);
            }

            pfarFree();
        }

        FreeLibrary(hLib);
    }
}