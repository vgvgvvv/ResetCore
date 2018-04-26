//
// Created by 董宸 on 2018/4/18.
//

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

int main(){
    // point to the relevant directories of the Mono installation
    mono_set_dirs("./mono/lib",
                  "./mono/etc");

    // load the default Mono configuration file in 'etc/mono/config'
    mono_config_parse(nullptr);

    MonoDomain* monoDomain = mono_jit_init_version("embedding_mono_domain",
                                                   "v4.0.30319");

// open our Example.dll assembly
    MonoAssembly* assembly = mono_domain_assembly_open(monoDomain,
                                                       "Example.dll");
    MonoImage* monoImage = mono_assembly_get_image(assembly);
    return 0;
}