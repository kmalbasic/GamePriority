#pragma once
#include <windows.h>
#include <TlHelp32.h>

enum priority {
    LOW = 0x00000040,
    BELOW_NORMAL = 0x00004000,
    NORMAL = 0x00000020,
    HIGH = 0x00000080,
    REALTIME = 0x00000100
};

std::wstring s_2_ws(const std::string& s)
{
    int len;
    int s_length = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s_length, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), s_length, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

HANDLE get_game_handle(LPWSTR process_name)
{
    PROCESSENTRY32 proc_32;
    HANDLE hSnap;
    if (hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (hSnap == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    else
    {
        proc_32.dwSize = sizeof(PROCESSENTRY32);
        while ((Process32Next(hSnap, &proc_32)) == TRUE)
        {
            if (_wcsicmp(proc_32.szExeFile, process_name) == 0)
            {
                HANDLE h = OpenProcess(PROCESS_SET_INFORMATION, TRUE, proc_32.th32ProcessID);
                return h;
            }
        }
        return 0;
    }
}

bool is_game_running(const wchar_t* process_name)
{
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry))
        while (Process32Next(snapshot, &entry))
            if (!_wcsicmp(entry.szExeFile, process_name))
                exists = true;

    CloseHandle(snapshot);
    return exists;
}

void set_priority(int priority, HANDLE process_handle) {
    SetPriorityClass(process_handle, priority);
}

DWORD get_priority(HANDLE process_handle) {
    return GetPriorityClass(process_handle);
}

void set_rest_priorities(int priority, LPWSTR process_name) {
    PROCESSENTRY32 proc_32;
    HANDLE snapshot_handle;
    if (snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
    if (snapshot_handle == INVALID_HANDLE_VALUE)
    {
        CloseHandle(snapshot_handle);
        return;
    }
    else
    {
        proc_32.dwSize = sizeof(PROCESSENTRY32);
        while ((Process32Next(snapshot_handle, &proc_32)) == TRUE)
        {
            if (_wcsicmp(proc_32.szExeFile, process_name) != 0)
            {
                HANDLE h = OpenProcess(PROCESS_SET_INFORMATION, TRUE, proc_32.th32ProcessID);
                set_priority(priority, h);
                CloseHandle(h);
            }
        }
        CloseHandle(snapshot_handle);
    }
}

