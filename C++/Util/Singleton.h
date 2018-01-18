//
// Created by 董宸 on 07/01/2018.
//

#ifndef __RESETCORE_SINGLETON_H__
#define __RESETCORE_SINGLETON_H__

namespace ResetCore{
    template < typename T >
class Singleton {
public:
    static T& GetInstance() {
        static MemGuard g; // clean up on program end
        if (!m_instance) {
            m_instance = new T();
        }
        return *m_instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    Singleton() { };
    virtual ~Singleton() { }

private:
    static T * m_instance;

    class MemGuard {
    public:
        ~MemGuard() {
            delete m_instance;
            m_instance = nullptr;
        }
    };
};

template < typename T >
T* Singleton<T>::m_instance = nullptr;
}


#endif //__RESETCORE_SINGLETON_H__
