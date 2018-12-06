#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "logger/ILogger.h"

namespace ReGL
{
    class Context
    {
    public:
        template<typename T>
        static void RegisterInterface(T* target);

        template<typename T>
        static const T& GetInterface();
        
        static void RegisterLogger(ILogger* logger);

        static const ILogger& GetLogger();


    private:
        static ILogger* logger_;
    };

}


#endif