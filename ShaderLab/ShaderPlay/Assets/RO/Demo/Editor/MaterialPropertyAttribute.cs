using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MaterialPropertyAttribute : Attribute
{

	public string Keyword { get; private set; }

	public MaterialPropertyAttribute(string keyword)
	{
		Keyword = keyword;
	}
}
