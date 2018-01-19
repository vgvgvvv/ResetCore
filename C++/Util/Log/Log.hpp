//
// Created by 董宸 on 20/01/2018.
//

#ifndef RESETCORE_LOG_HPP
#define RESETCORE_LOG_HPP


#include <string>

namespace ResetCore{
    class Log {
    public:
        static void LogInfo(std::string log);
        static void LogWarning(std::string log);
        static void LogError(std::string log);
    private:
        static std::string log_path_;
    };
}



#endif //RESETCORE_LOG_HPP
