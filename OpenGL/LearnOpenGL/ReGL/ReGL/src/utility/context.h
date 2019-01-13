#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "logger/ILogger.h"
#include <unordered_map>
#include "logger/ConsoleLogger.h"

namespace ReGL
{
    class Context
    {
    public:

        static void Init();

        static void RegisterLogger(ILogger* logger);

        static const ILogger& GetLogger();


    private:
        static std::unordered_map<type_info, void*> obj_map_;
        static ConsoleLogger default_logger_;
    };

    template<typename T>
    class ContextValue
    {
    public:
        static T& Get()
        {
            return *value_;
        };
        static void Set(T* value)
        {
            value_ = value;
        }
    private:
        static T* value_;
    };

    template<typename T>
    T* ContextValue<T>::value_ = nullptr;
}


#endif