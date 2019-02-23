using System;
using System.Reflection;
using Mono.Cecil;
using Mono.Cecil.Cil;

namespace CodeInject
{
    public abstract class MethodInjector
    {
        public void InjectIL(MethodDefinition methodDefinition)
        {
            var processor = methodDefinition.Body.GetILProcessor();
        }
    }
}