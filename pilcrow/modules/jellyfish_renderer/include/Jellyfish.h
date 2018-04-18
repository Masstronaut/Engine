#pragma once
#include "GLWindow.h" //OpenGL Windowing



#include <string>
namespace Jellyfish
{
	//-------------------
	// * API SELECTION *
	//-------------------
	//enum eRenderAPI {E_GL, E_DX11, E_DXR, E_VKN, E_DX12};
	//eRenderAPI g_API = E_GL; ///TODO: data-driven API choice

	
	//-------------------
	// * Testing Stub *
	//-------------------
	//basic linker test
	class Derp
	{
	public:
		void DoAThing();
	private:
		std::string str = "Oh look a jellyfish!";
	};

	class RenderableObject
	{
		int stub;
	};


	class Renderer
	{
		void Draw() {}
	};
}
