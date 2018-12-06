#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

namespace ReGL
{
    class ILogger
    {
    public:
        ILogger() = default;
        ILogger(const ILogger&) = default;
        ILogger& operator=(const ILogger&) = default;
        virtual void Log(const std::string&) const = 0;
        virtual void Warning(const std::string&) const = 0;
        virtual void Error(const std::string&) const = 0;
    };
}


#endif
