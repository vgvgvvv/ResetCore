namespace ReFrame.Util
{

    public interface IDataSerialize<T>
    {
        bool Read(out T value);

        bool Write(T value);
    }
}