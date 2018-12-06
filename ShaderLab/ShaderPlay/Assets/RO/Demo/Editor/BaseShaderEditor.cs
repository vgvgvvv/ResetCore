using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using UnityEditor;
using UnityEngine;

public class BaseShaderEditor : ShaderGUI {

	public void FindProperty(Material material, MaterialProperty[] props)
	{
		var propertyAttrs = GetType().GetFields().Where((f) =>
		{
			if (f.FieldType != typeof(MaterialProperty))
			{
				return false;
			}
			var attr = f.GetCustomAttribute(typeof(MaterialPropertyAttribute));
			return attr != null;
		}).Select((f) =>
		{
			return f.GetCustomAttribute(typeof(MaterialPropertyAttribute)) as MaterialPropertyAttribute;
		});
		
		foreach (var attr in propertyAttrs)
		{
			if(attr == null)
				continue;
			if (material.HasProperty(attr.Keyword))
			{
				FindProperty(attr.Keyword, props);
			}
		}
	}

	public override void OnGUI(MaterialEditor materialEditor, MaterialProperty[] properties)
	{
		base.OnGUI(materialEditor, properties);

		var editor = materialEditor;
		var material = materialEditor.target as Material;
		
		FindProperty(material, properties);
		
	}
}
