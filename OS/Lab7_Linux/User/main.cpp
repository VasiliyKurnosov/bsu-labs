#include <iostream>
#include <string>
#include <cstring>
#include <spawn.h>
#include <unistd.h>

extern char** environ;

const std::string ENVIRONMENT_VARIABLE = "PROC_TO_KILL";

int main()
{
    setenv(ENVIRONMENT_VARIABLE.c_str(), "firefox;thunderbird", 1);

    pid_t process_to_kill_pid;
    std::string process_to_kill_path = "../../SimpleProcess/cmake-build-debug/SimpleProcess";
    char* process_to_kill_argv[] = {"SimpleProcess", nullptr };
    int status = posix_spawn(&process_to_kill_pid, process_to_kill_path.c_str(), nullptr, nullptr, process_to_kill_argv, environ);
    if (status != 0)
    {
        std::cerr << "Failed to create process: " << strerror(status) << std::endl;
    }
    char temp[1000];
    sprintf(temp, "%d", process_to_kill_pid);
    char* killer_argv[] = {"Killer", "--id", temp, nullptr };
    int killer_pid;
    std::string killer_path = "../../Killer/cmake-build-debug/Killer";
    status = posix_spawn(&killer_pid, killer_path.c_str(), nullptr, nullptr, killer_argv, environ);
    if (status != 0)
    {
        std::cerr << "Failed to create process: " << strerror(status) << std::endl;
    }
    sleep(1);
    posix_spawn(&killer_pid, killer_path.c_str(), nullptr, nullptr, killer_argv, environ);

    status = posix_spawn(&process_to_kill_pid, process_to_kill_path.c_str(), nullptr, nullptr, process_to_kill_argv, environ);
    if (status != 0)
    {
        std::cerr << "Failed to create process: " << strerror(status) << std::endl;
    }
    killer_argv[1] = "--name";
    killer_argv[2] = "SimpleProcess";
    status = posix_spawn(&killer_pid, killer_path.c_str(), nullptr, nullptr, killer_argv, environ);
    if (status != 0)
    {
        std::cerr << "Failed to create process: " << strerror(status) << std::endl;
    }
    sleep(1);
    posix_spawn(&killer_pid, killer_path.c_str(), nullptr, nullptr, killer_argv, environ);

    unsetenv(ENVIRONMENT_VARIABLE.c_str());

    return 0;
}
