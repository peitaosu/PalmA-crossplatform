#include "foreground_win.h"
#include <locale>
#include <Windows.h>
#include "tlhelp32.h"
#include <string>

LPWSTR GetProcessNameById(DWORD ProcessID)
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    LPWSTR result = new WCHAR[32];

    //take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hProcessSnap) return(FALSE);

    pe32.dwSize = sizeof(PROCESSENTRY32);

    //retrieve information about the first process,
    //and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        //clean the snapshot object
        CloseHandle(hProcessSnap);
        return(NULL);
    }

    //matching Process ID
    do
    {
        if (ProcessID == pe32.th32ProcessID)
        {
            //copy Process Name
            wcscpy(result, (LPWSTR)pe32.szExeFile);
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    //clean the snapshot object
    CloseHandle(hProcessSnap);

    return result;
}

Foreground::Foreground()
{
    //set language
    setlocale(LC_ALL, "");
}


std::string Foreground::getForegroundProcessName(){

    //new variable process_ID
    DWORD process_ID = 0;

    //get foreground window
    HWND hwnd_foreground_window = GetForegroundWindow();

    //get foreground window process id
    GetWindowThreadProcessId(hwnd_foreground_window,&process_ID);

    //set buffer
    WCHAR text[MAX_PATH] = { 0 };

    //get foreground window window text
    GetWindowText(GetForegroundWindow(), text, MAX_PATH);

    //new variable szProcessName，used to save process name
    LPWSTR szProcessName = new WCHAR[32];
    wcscpy(szProcessName, GetProcessNameById(process_ID));


    std::wstring w_string(text);

    // convert wchar* to String
    std::string process_name(w_string.begin(), w_string.end());

    return process_name;
}

std::string Foreground::getForegroundWindowName(){

    //set buffer
    WCHAR text[MAX_PATH] = { 0 };

    //get foreground window text
    if(GetWindowText(GetForegroundWindow(), text, MAX_PATH)){
        //TODO: LOG ERROR;
    }

    std::wstring w_string(text);

    // convert wchar* to String
    std::string window_name(w_string.begin(), w_string.end());

    return window_name;

}



