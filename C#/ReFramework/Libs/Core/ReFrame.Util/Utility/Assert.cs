using System;

namespace ReFrame.Util
{
    public static class Assert
    {
        public static void IsNull(object obj)
        {
            if (obj == null)
            {
                throw new NullReferenceException();
            }
        }

        public static void True(bool value)
        {
            if (!value)
            {
                throw new Exception("意外的bool错误");
            }
        }
    }
}