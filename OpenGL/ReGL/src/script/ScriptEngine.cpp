//
// Created by ¶­å· on 2018/7/29.
//

#include "ScriptEngine.hpp"
#include <iostream>
#include <LogEngine.hpp>

namespace ReGL{
    bool ScriptEngine::Init() {
        mono_config_parse (nullptr);
        mono_domain_ = mono_jit_init_version("embedding_mono_domain",
                                                       "v4.0.30319");
        if(mono_domain_ == nullptr){
            LogEngine::Error("create monoDomain failed");
            return false;
        }
        return true;
    }

    bool ScriptEngine::Update() {
        return true;
    }

    bool ScriptEngine::UnInit() {
        mono_jit_cleanup(mono_domain_);
        return true;
    }

    bool ScriptEngine::OpenAssembly(const std::string& assemblyname, MonoAssembly *assembly) {
        // open our Example.dll assembly
        assembly = mono_domain_assembly_open(mono_domain_, assemblyname.c_str());
        if(assembly == nullptr){
            LogEngine::Error("create assembly failed");
            return false;
        }
        return true;

    }
}