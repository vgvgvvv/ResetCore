//
// Created by ¶­å· on 2018/7/29.
//

#ifndef REGL_LOGENGINE_HPP
#define REGL_LOGENGINE_HPP

#include <string>
using namespace std;

namespace ReGL{
    class LogEngine {
    public:
        LogEngine() = delete;
        ~LogEngine() = delete;

        static void Init();
        static void Uninit();

        static void Log(const string& log);
        static void Warning(const string& log);
        static void Error(const string& log);
    };
}



#endif //REGL_LOGENGINE_HPP
