#include <windows.h>
#include "Common/Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ReD3D::Application App(hInstance);
	if (!App.Init())
		return 0;
	return App.Run();
}
