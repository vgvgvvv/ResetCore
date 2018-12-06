#include "ConsoleLogger.h"
#include <iostream>

namespace ReGL
{
    HANDLE ConsoleLogger::hConsole = { GetStdHandle(STD_OUTPUT_HANDLE) };

    void ConsoleLogger::Log(const std::string& log) const
    {
        SetConsoleTextAttribute(hConsole, LOG_COLOR);
        std::cout << "[LOG] " << log << std::endl << "\t[FILE]" << __FILE__ << "\t[LINE]" << __LINE__ << std::endl;
    }

    void ConsoleLogger::Warning(const std::string& log) const
    {
        SetConsoleTextAttribute(hConsole, WARNING_COLOR);
        std::cout << "[WARNING] " << log << std::endl << "\t[FILE]" << __FILE__ << "\t[LINE]" << __LINE__ << std::endl;
    }

    void ConsoleLogger::Error(const std::string& log) const
    {
        SetConsoleTextAttribute(hConsole, ERROR_COLOR);
        std::cerr << "[ERROR] " << log << std::endl << "\t[FILE]" << __FILE__ << "\t[LINE]" << __LINE__ << std::endl;
    }
}
