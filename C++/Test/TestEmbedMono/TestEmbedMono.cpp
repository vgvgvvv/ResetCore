#include <iostream>
#include "mono/metadata/mono-config.h"
#include "mono/metadata/assembly.h"
#include "mono/jit/jit.h"

int main() {

//    mono_set_dirs("./mono/lib",
//                   "./mono/etc");
    mono_config_parse (NULL);
    // open our Example.dll assembly

    MonoDomain* monoDomain = mono_jit_init_version("embedding_mono_domain",
                                                   "v4.0.30319");

    if(monoDomain == nullptr){
        std::cerr << "create monoDomain failed" << std::endl;
        return 1;
    }

    // open our Example.dll assembly
    MonoAssembly* assembly = mono_domain_assembly_open(monoDomain,
                                                       "TestCSLib.dll");
    if(assembly == nullptr){
        std::cerr << "create assembly failed" << std::endl;
        return 1;
    }

    MonoImage* monoImage = mono_assembly_get_image(assembly);

    MonoClass* entityClass = mono_class_from_name(monoImage,
                                                  "TestCSLib",
                                                  "TestDll");

    MonoObject* instance = mono_object_new(monoDomain, entityClass);

    // find the Entity class constructor method that takes one parameter
    MonoMethod* constructorMethod = mono_class_get_method_from_name(entityClass,
                                                                    ".ctor",
                                                                    0);

    MonoObject* exception = NULL;
    mono_runtime_invoke(constructorMethod, instance, nullptr, &exception);

    // find the Process method that takes zero parameters
    MonoMethod* processMethod = mono_class_get_method_from_name(entityClass,
                                                                "Add",
                                                                2);

    exception = nullptr;

    void* args[2];
    args[0] = new int(5);
    args[1] = new int(100);

    // invoke the method
    // if invoking static methods, then the second argument must be NULL
    MonoObject* result = mono_runtime_invoke(processMethod, instance, args, &exception);


    // check for any thrown exception
    if(exception)
    {
        std::cout << mono_string_to_utf8(mono_object_to_string(exception, nullptr))
                  << std::endl;
    }

    int int_result = *(int*)mono_object_unbox (result);

    std::cout << int_result << std::endl;


    // shutdown mono
    mono_jit_cleanup(monoDomain);

    return 0;
}