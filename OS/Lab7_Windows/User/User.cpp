#include <iostream>
#include <windows.h>
#include <WtsApi32.h>
#include <cstring>

bool CheckProcess(DWORD pid);
bool CheckProcess(const TCHAR name[]);

int main()
{
    SetEnvironmentVariable(TEXT("PROC_TO_KILL"), TEXT("browser.exe;Telegram.exe"));

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    SECURITY_ATTRIBUTES sa_process;
    sa_process.nLength = sizeof(sa_process);
    sa_process.lpSecurityDescriptor = NULL;
    sa_process.bInheritHandle = FALSE;
    SECURITY_ATTRIBUTES sa_thread;
    sa_thread.nLength = sizeof(sa_thread);
    sa_thread.lpSecurityDescriptor = NULL;
    sa_thread.bInheritHandle = TRUE;
    TCHAR process_command_line[MAX_PATH];

    lstrcpy(process_command_line, TEXT("SimpleProcess"));
    PROCESS_INFORMATION pi_process_to_kill;
    if (!CreateProcess(NULL, process_command_line, &sa_process, &sa_thread, FALSE, 0, NULL, NULL, &si, &pi_process_to_kill))
    {
        std::cout << "Failed to create process: " << GetLastError() << std::endl;
    }
    DWORD pid = pi_process_to_kill.dwProcessId;
    wsprintf(process_command_line, TEXT("Killer --id %i"), pid);
    PROCESS_INFORMATION pi_killer;
    if (!CreateProcess(NULL, process_command_line, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi_killer))
    {
        std::cout << "Failed to create Killer process: " << GetLastError() << std::endl;
    }
    WaitForSingleObject(pi_process_to_kill.hProcess, INFINITE);
    if (!CheckProcess(pid))
    {
        std::cout << "There is no process with id " << pid << std::endl;
    }

    Sleep(1000);
    lstrcpy(process_command_line, TEXT("SimpleProcess"));
    if (!CreateProcess(NULL, process_command_line, &sa_process, &sa_thread, FALSE, 0, NULL, NULL, &si, &pi_process_to_kill))
    {
        std::cout << "Failed to create process: " << GetLastError() << std::endl;
    }
    lstrcpy(process_command_line, TEXT("Killer --name SimpleProcess.exe"));
    if (!CreateProcess(NULL, process_command_line, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi_killer))
    {
        std::cout << "Failed to create Killer process: " << GetLastError() << std::endl;
    }
    WaitForSingleObject(pi_process_to_kill.hProcess, INFINITE);
    if (!CheckProcess(TEXT("SimpleProcess.exe")))
    {
        std::cout << "There is no process with name SimpleProcess.exe" << std::endl;
    }

    CloseHandle(pi_process_to_kill.hProcess);
    CloseHandle(pi_process_to_kill.hThread);
    CloseHandle(pi_killer.hProcess);
    CloseHandle(pi_killer.hThread);

    SetEnvironmentVariable(TEXT("PROC_TO_KILL"), NULL);

    return 0;
}

bool CheckProcess(DWORD pid)
{
    WTS_PROCESS_INFO* process_info_ptr;
    DWORD processes_number;
    if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, 0, 1, &process_info_ptr, &processes_number) != 0)
    {
        for (DWORD i = 0; i < processes_number; ++i)
        {
            if (process_info_ptr[i].ProcessId == pid)
            {
                return true;
            }
        }
    }
    return false;
}

bool CheckProcess(const TCHAR name[])
{
    WTS_PROCESS_INFO* process_info_ptr;
    DWORD processes_number;
    if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, 0, 1, &process_info_ptr, &processes_number) != 0)
    {
        for (DWORD i = 0; i < processes_number; ++i)
        {
            if (lstrcmp(process_info_ptr[i].pProcessName, name) == 0)
            {
                return true;
            }
        }
    }
    return false;
}
