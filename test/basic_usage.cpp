#include "../logger.h"
#include <thread>
using namespace std;

int main()
{
    logger log;
    log << "Hello World!!";
    log << "What is your name?";
    string cur = get_current_working_directory();

    logger log2("test.txt");
    log2 << "logger test";
    log2 << "Before sleep";
    std::this_thread::sleep_for(2s);
    log2 << "After sleep";
}