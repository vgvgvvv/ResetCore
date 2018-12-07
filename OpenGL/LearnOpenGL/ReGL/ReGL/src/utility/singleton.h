#ifndef __SINGLETON_H__
#define __SINGLETON_H__

namespace ReGL
{
    template <typename T>
    class Singleton
    {
    public:
        static T& Instance()
        {
            if (instance_ == nullptr)
            {
                instance_ = new T;
            }
            return instance_;
        }
    protected:
        Singleton() {}
    private:
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        static T* instance_;
    };

    template<typename T>
    T* Singleton<T>::instance_ = nullptr;
}

#endif //__SINGLETON_H__
