namespace FrameSync.Managers
{
    public interface IManager
    {
        void Init();
        void Update();
        void LateUpdate();
        void FixedUpdate();
        void UnInit();
    }
}