#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <csignal>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

const std::string PROC_DIRECTORY = "/proc/";
const std::string ENVIRONMENT_VARIABLE = "PROC_TO_KILL";

std::vector<pid_t> FindAllProcessIDsByName(const std::string& name);
bool IsNumeric(const std::string& str);

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Killer process incorrect arguments" << std::endl;
        return 0;
    }
    std::vector<pid_t> processes_to_kill_ids;
    if (strcmp(argv[1], "--id") == 0)
    {
        processes_to_kill_ids.push_back(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "--name") == 0)
    {
        std::string name(argv[2]);
        processes_to_kill_ids = FindAllProcessIDsByName(name);
    }

    if (getenv(ENVIRONMENT_VARIABLE.c_str()) != nullptr)
    {
        std::string environment_variable_str(getenv(ENVIRONMENT_VARIABLE.c_str()));
        std::stringstream string_stream(environment_variable_str);
        std::string single_process_name;
        char delimiter = ';';
        while (std::getline(string_stream, single_process_name, delimiter))
        {
            std::vector<pid_t> process_ids = FindAllProcessIDsByName(single_process_name);
            processes_to_kill_ids.insert(processes_to_kill_ids.end(), process_ids.begin(), process_ids.end());
        }
    }

    for (auto pid : processes_to_kill_ids)
    {
        if (kill(pid, 2) == 0)
        {
            std::cout << "Process " << pid << " terminated" << std::endl;
        }
        else
        {
            std::cout << "Failed to terminate process " << pid << std::endl;
        }
    }

    return 0;
}

std::vector<pid_t> FindAllProcessIDsByName(const std::string& name)
{
    std::vector<pid_t> result;
    DIR* dirp = opendir(PROC_DIRECTORY.c_str());
    if (dirp == nullptr)
    {
        std::cerr << "Can't open /proc/ directory" << std::endl;
        return std::vector<pid_t>();
    }
    dirent* directory_entity;
    while ((directory_entity = readdir(dirp)) != nullptr)
    {
        if (directory_entity->d_type == DT_DIR)
        {
            std::string directory_name(directory_entity->d_name);
            if (IsNumeric(directory_name))
            {
                std::string path = PROC_DIRECTORY + directory_name + "/cmdline";
                std::ifstream fin(path);
                if (fin.is_open())
                {
                    std::string process_name;
                    fin >> process_name;
                    int position = process_name.find('\0');
                    if (position != std::string::npos)
                    {
                        process_name = process_name.substr(0, position);
                    }
                    position = process_name.find_last_of('/');
                    if (position != std::string::npos)
                    {
                        process_name.erase(0, position + 1);
                    }
                    if (process_name == name)
                    {
                        result.push_back((pid_t)std::stoi(directory_name));
                    }
                }
            }
        }
    }
    closedir(dirp);
    return result;
}

bool IsNumeric(const std::string& str)
{
    for (char c : str)
    {
        if (c < '0' || c > '9')
        {
            return false;
        }
    }
    return true;
}
