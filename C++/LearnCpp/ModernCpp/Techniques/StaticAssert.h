//
// Created by 35207 on 2019/11/30 0030.
//

#ifndef RESETCORE_STATICASSERT_H
#define RESETCORE_STATICASSERT_H

template <bool> struct CompileTimeError{
    CompileTimeError(...); //任意参数表
};
template <> struct CompileTimeError<false>{};

// 下面的ERROR_##msg用于显式表示报错信息
// 实际的STATIC_CHECK宏
#define STATIC_CHECK(expr, msg) \
    { \
        class ERROR_##msg {}; \
        (void)sizeof(CompileTimeChecker<(expr)>(ERROR_##msg()));\
    }

#endif //RESETCORE_STATICASSERT_H
