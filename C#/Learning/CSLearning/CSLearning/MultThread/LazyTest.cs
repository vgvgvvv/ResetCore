using System;

namespace CSLearning.MultThread
{
    public class LazyTest
    {
        private Lazy<int> lazy = new Lazy<int>(() => 100);

        public void Test()
        {

        }
    }
}