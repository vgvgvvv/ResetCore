namespace ReFrame.Util
{
    public static class ContextValue<T>
    {
        public static T Value { get; private set; }

        public static void Set(T value)
        {
            Value = value;
        }

        
    }
}