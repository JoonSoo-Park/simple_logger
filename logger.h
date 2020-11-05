#pragma once
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#ifdef _WIN32
#include <direct.h>
#elif __linux__
#include <unistd.h>
#endif

/*
 * Generate uuid in format of 8-4-4-12
 */
namespace uuid_generator{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> distrib(0, 15);

    std::string generate_uuid()
    {
        std::stringstream ss;

        ss << std::uppercase << std::hex;
        for (int i = 0; i < 8; ++i) {
            ss << distrib(gen);
        }

        ss << "-";
        for (int i = 0; i < 4; ++i) {
            ss << distrib(gen);
        }

        ss << "-";
        for (int i = 0; i < 4; ++i) {
            ss << distrib(gen);
        }

        ss << "-";
        for (int i = 0; i < 12; ++i) {
            ss << distrib(gen);
        }

        return ss.str();
    }
}

/* 
 * General purpos functions
 */
std::string get_current_working_directory();


/*
 * logger class
 */
class logger
{
public:
    logger();
    ~logger(); 

    int persist(const char*);
    logger& operator<<(std::string);
private:
    std::string name;
    std::string logpath;
    std::ofstream logfile;
};

logger::logger()
{
    using namespace uuid_generator;

    name = generate_uuid();
    std::string cur_work_dir = get_current_working_directory();

    // logpath = log file path in current directory
    logpath = cur_work_dir + "/" + name + ".tmp";
    std::cout << "Temp file name: " << logpath << std::endl;
    
    // open file for write|truncate
    logfile.open(logpath.c_str(), std::ios::out | std::ios::trunc);
}

// close opened logfile
logger::~logger()
{
    if(logfile.is_open()) logfile.close();
}

logger& logger::operator<< (std::string msg)
{
    std::time_t now = time(0);
    std::tm* Itm = localtime(&now);

    std::stringstream ss;
    ss << "[" << 1900 + Itm->tm_year 
       << "-" << 1 + Itm->tm_mon 
       << "-" << Itm->tm_mday << " ";

    ss << Itm->tm_hour << ":" 
       << Itm->tm_min << ":" 
       << Itm->tm_sec << "] ";

    ss << msg << '\n';

    logfile << ss.str();
    return *this;
}

int logger::persist(const char* new_path)
{
    logfile.close();
    std::string temp = std::string(new_path) + "/" + name + ".txt";
    std::rename(logpath.c_str(), temp.c_str());

    return 1;
}

std::string get_current_working_directory()
{
    char buff[FILENAME_MAX];
    #ifdef _WIN32
    if (_getcwd(buff, FILENAME_MAX) != 0)
        return string(buff);
    #elif __linux__
    if (getcwd(buff, FILENAME_MAX) != 0)
        return std::string(buff);
    #else
    // throw error
    return std::string("");
    #endif
}