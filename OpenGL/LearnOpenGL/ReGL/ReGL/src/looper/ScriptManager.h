#ifndef __SCRIPTMANAGER_H__
#define __SCRIPTMANAGER_H__

namespace ReGL
{
    class ScriptManager
    {
    public:
        bool Init();
        bool PreUpdate();
        bool Update();
        bool LateUpdate();
        bool Uninit();
    };
}

#endif // __SCRIPTMANAGER_H__
