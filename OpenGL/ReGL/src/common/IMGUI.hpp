//
// Created by ¶­å· on 2018/7/26.
//

#ifndef REGL_IMGUI_HPP
#define REGL_IMGUI_HPP

namespace ReGL{
    class IMGUI {
    public:
        IMGUI() = delete;
        ~IMGUI() = delete;
        static bool Init();
        static bool OnGUI();
        static bool Uninit();
    };
}




#endif //REGL_IMGUI_HPP
