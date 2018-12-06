
#ifndef __WORLD_H__
#define __WORLD_H__

namespace ReGL
{
    class World
    {
    public:
        World() = delete;
        ~World() = delete;

        static bool Init();
        static bool PreUpdate();
        static bool Update();
        static bool LateUpdate();
        static bool Uninit();

        static bool Run(int argc, char **argv);
    
    private:
        static bool running_;
    };
}

#endif //__WORLD_H__