//
// Created by ¶­å· on 2018/7/26.
//

#ifndef REGL_APPLICATION_HPP
#define REGL_APPLICATION_HPP

#include "Window.hpp"
#include "Common.hpp"

namespace ReGL{
    class Application {
    public:
        Application();
        ~Application();
    public:
        bool Init();
        bool Run();
        bool Quit();
    public:
        static Application* Current(){ return current; }
        static Window* MainWindow(){ return main_window; }
    private:
        static Application* current;
        static Window* main_window;
    };
}



#endif //REGL_APPLICATION_HPP
