
#ifndef __CONSOLE_LOGGER_H__
#define __CONSOLE_LOGGER_H__
#include "ILogger.h"
#include <Windows.h>

namespace ReGL
{
    class ConsoleLogger : public ILogger
    {
    public:
        ConsoleLogger() = default;

        void Log(const std::string&) const override final;
        void Warning(const std::string&) const override final;
        void Error(const std::string&) const override final;
    private:
        static HANDLE hConsole;
        const int LOG_COLOR = 15;
        const int WARNING_COLOR = 14;
        const int ERROR_COLOR = 12;
    };
}


#endif
