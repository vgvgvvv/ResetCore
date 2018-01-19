//
// Created by 董宸 on 20/01/2018.
//

#ifndef RESETCORE_NONCOPYABLE_HPP
#define RESETCORE_NONCOPYABLE_HPP

/**
 * 继承该类则无法被拷贝
 */
class NonCopyable
{
public:
    NonCopyable() {}

private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};

#endif //RESETCORE_NONCOPYABLE_HPP
