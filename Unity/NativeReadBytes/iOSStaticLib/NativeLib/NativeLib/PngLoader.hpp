//
//  PngLoader.hpp
//  NativeLib
//
//  Created by 董宸 on 2018/11/8.
//  Copyright © 2018 董宸. All rights reserved.
//

#ifndef PngLoader_hpp
#define PngLoader_hpp

#include <stdio.h>
#include <stdlib.h>

class PngLoader {
public:
    uintptr_t texture;
    uint32_t width = 0;
    uint32_t height = 0;
};

#endif /* PngLoader_hpp */
