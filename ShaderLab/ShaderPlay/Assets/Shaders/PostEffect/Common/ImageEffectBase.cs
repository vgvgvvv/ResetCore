using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace ResetCore.ShaderPlay
{
    /// <summary>
    /// 指定Shader
    /// </summary>
    [AttributeUsage(AttributeTargets.Class, Inherited = true, AllowMultiple = false)]
    sealed class ImageEffectShaderAttribute : Attribute
    {
        public readonly string shaderName;

        public ImageEffectShaderAttribute(string shaderName)
        {
            this.shaderName = shaderName;
        }

    }
    
    /// <summary>
    /// 基础后处理类
    /// </summary>
    [ExecuteInEditMode]
    public abstract class ImageEffectBase : MonoBehaviour
    {

        /// Provides a shader property that is set in the inspector
        /// and a material instantiated from the shader
        private Shader _shader;
        private Material m_Material;

        protected virtual void Start()
        {
            // Disable if we don't support image effects
            if (!SystemInfo.supportsImageEffects)
            {
                enabled = false;
                return;
            }

            // Disable the image effect if the shader can't
            // run on the users graphics card
            if (!shader || !shader.isSupported)
            {
                enabled = false;
            }
        }

        protected Material material
        {
            get
            {
                if (m_Material == null && shader != null)
                {
                    m_Material = new Material(shader);
                    m_Material.hideFlags = HideFlags.HideAndDontSave;
                }
                return m_Material;
            }
        }

        public Shader shader
        {
            get
            {
                if (_shader == null)
                {
                    var shaderAttrs = GetType().GetCustomAttributes(typeof(ImageEffectShaderAttribute), true) as ImageEffectShaderAttribute[];
                    if (shaderAttrs != null && shaderAttrs.Length >= 0)
                    {
                        _shader = Shader.Find(shaderAttrs[0].shaderName);
                    }
                }
                return _shader;
            }
        }

        protected virtual void OnDisable()
        {
            if (m_Material)
            {
                DestroyImmediate(m_Material);
                m_Material = null;
            }
        }
    }
}

