// dllmain.cpp : Defines the entry point for the DLL application.


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "pjsua2.hpp"
using namespace std;
using namespace pj;

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


extern "C"
{
    extern void* CoTaskMemAlloc(long cb);

    static char* pjVersion = NULL;

    __declspec(dllexport) char* __stdcall PjGetVersion()
    {
        if (pjVersion == NULL)
        {
            auto version = pj_get_version();
            long ulSize = (long)(strlen(version) + sizeof(char));
            pjVersion = (char*)CoTaskMemAlloc(ulSize);
            strcpy(pjVersion, version);
        }
        return pjVersion;
    }

}
