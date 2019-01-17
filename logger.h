#ifndef LOGGER_H
#define LOGGER_H


#include <iostream>
#include <mutex>

class Logger {

private:
  std::mutex l_print;

public:
    void print(std::string const & s)
    {
        l_print.lock();
        std::cout << s << std::endl;
        l_print.unlock();
    }

    void print(std::string const & s1, std::string const & s2)
    {
        l_print.lock();
        std::cout << s1 << s2 << std::endl;
        l_print.unlock();
    }

};

#endif // LOGGER_H
