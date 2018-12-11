#include "context.h"
#include "logger/ConsoleLogger.h"
#include "looper/InputManager.h"
#include "opengl/GLFWInputManager.h"

namespace ReGL
{
    ConsoleLogger Context::default_logger_;

    void Context::Init()
    {
        RegisterLogger(&default_logger_);
        ContextValue<InputManager>::Set(new GLFWInputManager());
    }

    void Context::RegisterLogger(ILogger* logger)
    {
        ContextValue<ILogger>::Set(logger);
    }

    const ILogger& Context::GetLogger()
    {
        return ContextValue<ILogger>::Get();
    }

}
