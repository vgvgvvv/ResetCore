//
// Created by ¶­å· on 2018/7/29.
//

#ifndef REGL_SCRIPTENGINE_HPP
#define REGL_SCRIPTENGINE_HPP

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <string>

namespace ReGL{
    class ScriptEngine {
    public:
        ScriptEngine() = delete;
        ~ScriptEngine() = delete;

    public:
        static bool Init();
        static bool Uninit();

        static bool OpenAssembly(const std::string& assemblyname, MonoAssembly* assembly);

    private:
        static MonoDomain* mono_domain_;
    };
}



#endif //REGL_SCRIPTENGINE_HPP
