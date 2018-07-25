#include "Application.h"

namespace ReD3D
{
	Application::Application(HINSTANCE hInstance) : app_inst_(hInstance)
	{
		assert(app_inst_ == nullptr);
		current_ = this;
	}

	Application::~Application()
	{
	}

	bool Application::Init()
	{
	}

	bool Application::Run()
	{
	}
}
