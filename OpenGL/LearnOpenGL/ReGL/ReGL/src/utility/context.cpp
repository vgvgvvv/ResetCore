#include "context.h"
#include "logger/ConsoleLogger.h"

namespace ReGL
{
    ConsoleLogger Context::default_logger_;

    void Context::Init()
    {
        RegisterLogger(default_logger_);
    }

    void Context::RegisterLogger(ILogger& logger)
    {
        ContextValue<ILogger>::Set(logger);
    }

    const ILogger& Context::GetLogger()
    {
        return ContextValue<ILogger>::Get();
    }

}
