
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Common.h"
#include "Window.h"

namespace ReD3D
{
	class Application
	{
	public:
		Application(HINSTANCE hInstance);
		Application(const Application& other) = delete;
		Application& operator=(const Application& other) = delete;
		virtual ~Application();
		bool Init();
		bool Run();

		static const Application* Current() { return current_; }
		const HINSTANCE& AppInst() const { return app_inst_; }
	
	private:
		static Application* current_;
		Window main_window_;
		HINSTANCE app_inst_;
	};
}

#endif