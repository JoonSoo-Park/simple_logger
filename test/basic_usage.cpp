#include "./logger.h"
using namespace std;

int main()
{
    logger log;
    log << "Hello World!!";
    log << "What is your name?";
    string cur = get_current_working_directory();
    log.persist(cur.c_str());
}