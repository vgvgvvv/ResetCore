using System;
using UnityEngine;

namespace CSLearning.SpecialGenerics
{
    public class TestClass<T>
    {
        private static Action<TestClass<T>, T> testAct;
        private static Action<TestClass<T>, T> defaultAct = (self, e) => { Debug.Log("我是默认方法"); };
            
        static TestClass()
        {
            TestClass<bool>.testAct = (self, e) => { Debug.Log("我是bool类型的特化方法！"); };
            TestClass<int>.testAct = (self, e) => { Debug.Log("我是int类型的特化方法！"); };
            TestClass<float>.testAct = (self, e) => { Debug.Log("我是float类型的特化方法！"); };
            TestClass<string>.testAct = (self, e) => { Debug.Log("我是string类型的特化方法！"); };
        }

        public void TestMethod(T arg)
        {
            testAct = testAct ?? defaultAct;
            testAct(this, arg);
        }
    }
}