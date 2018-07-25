#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Common.h"

namespace ReD3D
{
	class Window
	{
	public:
		Window();
		~Window();
		Window(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) noexcept = delete;
		bool Init();
	private:

	};
}

#endif
