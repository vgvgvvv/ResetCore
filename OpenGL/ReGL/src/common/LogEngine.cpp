//
// Created by ¶­å· on 2018/7/29.
//

#include "LogEngine.h"
#include <iostream>

namespace ReGL{

    void LogEngine::Init() {

    }

    void LogEngine::Uninit() {

    }

    void LogEngine::Log(const string& log) {
        cout << log << endl;
    }

    void LogEngine::Warning(const string& log) {
        cout << log << endl;
    }

    void LogEngine::Error(const string& log) {
        std::cerr << log << std::endl;
    }
}