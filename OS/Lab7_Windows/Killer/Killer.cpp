#include <iostream>
#include <windows.h>
#include <WtsApi32.h>
#include <vector>
#include <string>
#include <sstream>

std::vector<DWORD> FindAllProcessIDsByName(TCHAR* name);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Killer process incorrect arguments" << std::endl;
		return 0;
	}
	std::vector<DWORD> processes_to_kill_ids;
	if (strcmp(argv[1], "--id") == 0)
	{
		processes_to_kill_ids.push_back(atoi(argv[2]));
	}
	else if (strcmp(argv[1], "--name") == 0)
	{
		std::string str(argv[2]);
		std::wstring w_str(str.begin(), str.end());
		TCHAR temp[1000];
		lstrcpy(temp, w_str.c_str());
		processes_to_kill_ids = FindAllProcessIDsByName(temp);
	}

	TCHAR environment_variable[1000];
	if (GetEnvironmentVariable(TEXT("PROC_TO_KILL"), environment_variable, sizeof(environment_variable)) != 0)
	{
		std::wstring w_str(environment_variable);
		std::wstringstream w_string_stream(w_str);
		std::wstring variable;
		TCHAR delimiter = ';';
		while (std::getline(w_string_stream, variable, delimiter))
		{
			TCHAR temp[1000];
			lstrcpy(temp, variable.c_str());
			std::vector<DWORD> process_ids = FindAllProcessIDsByName(temp);
			processes_to_kill_ids.insert(processes_to_kill_ids.end(), process_ids.begin(), process_ids.end());
		}
	}

	for (auto pid : processes_to_kill_ids)
	{
		HANDLE h_process_to_kill = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (TerminateProcess(h_process_to_kill, 2))
		{
			std::cout << "Process " << pid << " terminated" << std::endl;
		}
		else
		{
			std::cout << "Failed to terminate process " << pid << std::endl;
		}
		CloseHandle(h_process_to_kill);
	}

	return 0;
}

std::vector<DWORD> FindAllProcessIDsByName(TCHAR* name)
{
	std::vector<DWORD> result;
	WTS_PROCESS_INFO* process_info_ptr;
	DWORD processes_number;
	if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, 0, 1, &process_info_ptr, &processes_number) != 0)
	{
		for (DWORD i = 0; i < processes_number; ++i)
		{
			std::wstring w_str(name);
			if (lstrcmp(process_info_ptr[i].pProcessName, w_str.c_str()) == 0)
			{
				result.push_back(process_info_ptr[i].ProcessId);
			}
		}
	}
	return result;
}
