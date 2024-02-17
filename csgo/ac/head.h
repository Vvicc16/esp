#pragma once
#include<iostream>
#include<Windows.h>
#include <tlhelp32.h>
#include <string>

//获取进程PID
DWORD GPid_Name(const std::wstring& processName) {
    DWORD pid = 0;
    // 创建进程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(processEntry);
        // 遍历进程快照
        if (Process32First(hSnapshot, &processEntry)) {
            do {
                // 检查进程名称是否匹配
                if (processName == processEntry.szExeFile) {
                    pid = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }
        CloseHandle(hSnapshot);
    }
    return pid;
}

//读模块基址
DWORD_PTR GetModuleBaseAddress(DWORD dwProcessId, const wchar_t* moduleName) {
    DWORD_PTR moduleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);
        if (Module32First(hSnapshot, &moduleEntry)) {
            do {
                if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
                    moduleBaseAddress = reinterpret_cast<DWORD_PTR>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }
        CloseHandle(hSnapshot);
    }
    return moduleBaseAddress;
}


//读内存
inline DWORD RPM_D(HANDLE hProcess, DWORD TargetAddr, BOOL* Status)
{
	BOOL fok=0;
	DWORD Result = 0;
	fok = ReadProcessMemory(hProcess, (LPCVOID)TargetAddr, &Result, 4, NULL);
	if (Status != 0) *Status = fok;
	return Result;
}

inline BYTE RPM_B(HANDLE hProcess, BYTE TargetAddr, BOOL* Status)
{
	BOOL fok = 0;
	BYTE Result = 0;
	fok = ReadProcessMemory(hProcess, (LPCVOID)TargetAddr, &Result, 1, NULL);
	if (Status != 0) *Status = fok;
	return Result;
}

inline FLOAT RPM_F(HANDLE hProcess, DWORD TargetAddr, BOOL* Status)
{
	BOOL fok = 0;
	FLOAT Result = 0;
	fok = ReadProcessMemory(hProcess, (LPCVOID)TargetAddr, &Result, 4, NULL);
	if (Status != 0) *Status = fok;
	return Result;
}

//写内存
inline BOOL WPM_D(HANDLE hProcess, DWORD TargetAddr, DWORD dwNum)
{
	return WriteProcessMemory(hProcess, (LPVOID)TargetAddr, &dwNum, 4, NULL);
}

inline BOOL WPM_B(HANDLE hProcess, DWORD TargetAddr, BYTE bwNum)
{
	return WriteProcessMemory(hProcess, (LPVOID)TargetAddr, &bwNum, 1, NULL);
}

inline BOOL WPM_F(HANDLE hProcess, DWORD TargetAddr, FLOAT fwNum)
{
	return WriteProcessMemory(hProcess, (LPVOID)TargetAddr, &fwNum, 4, NULL);
}

//读模块基址加多级偏移内存
inline DWORD RPMP_D(HANDLE hProcess, DWORD dwProcessId, const wchar_t* moduleName, BOOL* Status, DWORD ptr1, DWORD ptr2 = -1, DWORD ptr3 = -1, DWORD ptr4 = -1, DWORD ptr5 = -1)
{
    DWORD_PTR BaseAddr = GetModuleBaseAddress(dwProcessId, moduleName);
    DWORD_PTR ptr = BaseAddr + ptr1;
    DWORD_PTR Result = RPM_D(hProcess, ptr, NULL);
    if (ptr2 == -1) return Result;

    ptr = Result + ptr2;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr3 == -1) return Result;

    ptr = Result + ptr3;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr4 == -1) return Result;

    ptr = Result + ptr4;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr5 == -1) return Result;

    ptr = Result + ptr5;
    Result = RPM_D(hProcess, ptr, NULL);
    return Result;
}

inline BYTE RPMP_B(HANDLE hProcess, DWORD dwProcessId, const wchar_t* moduleName, BOOL* Status, DWORD ptr1, DWORD ptr2 = -1, DWORD ptr3 = -1, DWORD ptr4 = -1, DWORD ptr5 = -1)
{
    BYTE ults = 0;
    DWORD_PTR BaseAddr = GetModuleBaseAddress(dwProcessId, moduleName);
    DWORD_PTR ptr = BaseAddr + ptr1;
    DWORD_PTR Result = RPM_D(hProcess, ptr, NULL);
    if (ptr2 == -1) return Result;

    ptr = Result + ptr2;
    Result = RPM_D(hProcess, ptr, NULL);
    ults = RPM_B(hProcess, ptr, NULL);
    if (ptr3 == -1) return ults;

    ptr = Result + ptr3;
    Result = RPM_D(hProcess, ptr, NULL);
    ults = RPM_B(hProcess, ptr, NULL);
    if (ptr4 == -1) return ults;

    ptr = Result + ptr4;
    Result = RPM_D(hProcess, ptr, NULL);
    ults = RPM_B(hProcess, ptr, NULL);
    if (ptr5 == -1) return ults;

    ptr = Result + ptr5;
    Result = RPM_D(hProcess, ptr, NULL);
    ults = RPM_B(hProcess, ptr, NULL);
    return ults;
}

inline FLOAT RPMP_F(HANDLE hProcess, DWORD dwProcessId, const wchar_t* moduleName, BOOL* Status, DWORD ptr1, DWORD ptr2 = -1, DWORD ptr3 = -1, DWORD ptr4 = -1, DWORD ptr5 = -1)
{
    FLOAT ults = 0;
    DWORD_PTR BaseAddr = GetModuleBaseAddress(dwProcessId, moduleName);
    DWORD_PTR ptr = BaseAddr + ptr1;
    DWORD_PTR Result = RPM_D(hProcess, ptr, NULL);
    if (ptr2 == -1) return Result;

    ptr = Result + ptr2;
    Result = RPM_D(hProcess, ptr, NULL);
    ults = RPM_F(hProcess, ptr, NULL);
    if (ptr3 == -1) return ults;

    ptr = Result + ptr3;
    Result = RPM_D(hProcess, ptr, NULL);
    ults = RPM_F(hProcess, ptr, NULL);
    if (ptr4 == -1) return ults;

    ptr = Result + ptr4;
    Result = RPM_D(hProcess, ptr, NULL);
    ults = RPM_F(hProcess, ptr, NULL);
    if (ptr5 == -1) return ults;

    ptr = Result + ptr5;
    Result = RPM_D(hProcess, ptr, NULL);
    ults = RPM_F(hProcess, ptr, NULL);
    return ults;
}

//写模块基址加多级偏移内存
inline BOOL WPMP_D(HANDLE hProcess, DWORD dwProcessId, const wchar_t* moduleName, DWORD dwNum, DWORD ptr1, DWORD ptr2 = -1, DWORD ptr3 = -1, DWORD ptr4 = -1, DWORD ptr5 = -1)
{
    DWORD_PTR BaseAddr = GetModuleBaseAddress(dwProcessId, moduleName);
    DWORD_PTR ptr = BaseAddr + ptr1;
    DWORD_PTR Result = RPM_D(hProcess, ptr, NULL);
    if (ptr2 == -1) return WPM_D(hProcess, ptr, dwNum);

    ptr = Result + ptr2;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr3 == -1) return WPM_D(hProcess, ptr, dwNum);

    ptr = Result + ptr3;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr4 == -1) return WPM_D(hProcess, ptr, dwNum);

    ptr = Result + ptr4;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr5 == -1) return WPM_D(hProcess, ptr, dwNum);

    ptr = Result + ptr5;
    return WPM_D(hProcess, ptr, dwNum);
}

inline BOOL WPMP_B(HANDLE hProcess, DWORD dwProcessId, const wchar_t* moduleName, DWORD ptr1, DWORD ptr2 = -1, DWORD ptr3 = -1, DWORD ptr4 = -1, DWORD ptr5 = -1, BYTE bwNum = 0)
{
    DWORD_PTR BaseAddr = GetModuleBaseAddress(dwProcessId, moduleName);
    DWORD_PTR ptr = BaseAddr + ptr1;
    DWORD_PTR Result = RPM_D(hProcess, ptr, NULL);
    if (ptr2 == -1) return WPM_B(hProcess, ptr, bwNum);

    ptr = Result + ptr2;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr3 == -1) return WPM_B(hProcess, ptr, bwNum);

    ptr = Result + ptr3;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr4 == -1) return WPM_B(hProcess, ptr, bwNum);

    ptr = Result + ptr4;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr5 == -1) return WPM_B(hProcess, ptr, bwNum);

    ptr = Result + ptr5;
    return WPM_B(hProcess, ptr, bwNum);
}

inline BOOL WPMP_F(HANDLE hProcess, DWORD dwProcessId, const wchar_t* moduleName, DWORD ptr1, DWORD ptr2 = -1, DWORD ptr3 = -1, DWORD ptr4 = -1, DWORD ptr5 = -1, FLOAT fwNum = 0)
{
    DWORD_PTR BaseAddr = GetModuleBaseAddress(dwProcessId, moduleName);
    DWORD_PTR ptr = BaseAddr + ptr1;
    DWORD_PTR Result = RPM_D(hProcess, ptr, NULL);
    if (ptr2 == -1) return WPM_F(hProcess, ptr, fwNum);

    ptr = Result + ptr2;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr3 == -1) return WPM_F(hProcess, ptr, fwNum);

    ptr = Result + ptr3;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr4 == -1) return WPM_F(hProcess, ptr, fwNum);

    ptr = Result + ptr4;
    Result = RPM_D(hProcess, ptr, NULL);
    if (ptr5 == -1) return WPM_F(hProcess, ptr, fwNum);

    ptr = Result + ptr5;
    return WPM_F(hProcess, ptr, fwNum);
}