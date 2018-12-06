#include "Context.h"
#include "logger/ConsoleLogger.h"

namespace ReGL
{
    ILogger* Context::logger_ = {new ConsoleLogger()};

    template <typename T>
    void Context::RegisterInterface(T* target)
    {
    }

    template <typename T>
    const T& Context::GetInterface()
    {
    }


    void Context::RegisterLogger(ILogger* logger)
    {
        if(logger == nullptr)
        {
            return;
        }
        delete logger_;
        logger_ = logger;
    }

    const ILogger& Context::GetLogger()
    {
        return *logger_;
    }
}
