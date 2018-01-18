using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Main : MonoBehaviour
{

	private DemoWorld _world = new DemoWorld();
	
	private void Awake()
	{
		_world.Awake();
	}

	// Use this for initialization
	void Start () 
	{
		_world.Start();
	}
	
	// Update is called once per frame
	void Update () 
	{
		_world.Update();
	}
}
