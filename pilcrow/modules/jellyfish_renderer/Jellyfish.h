#pragma once

//-------------------------------------------------------------
// *** MAIN ENTRY POINT FOR THE JELLYFISH RENDERER LIBRARY ***
//-------------------------------------------------------------

// std
#include <string>

// ours
#include "Model.h"    //3D Model support
#include "iWindow.h"  //Windowing

// GL Implementations
#include "pilcrow/modules/jellyfish_renderer/GL/GLMesh.h"
#include "pilcrow/modules/jellyfish_renderer/GL/GLProgram.h"  //includes GLShader.h
#include "pilcrow/modules/jellyfish_renderer/GL/GLText.h"
#include "pilcrow/modules/jellyfish_renderer/GL/GLTexture.h"
#include "pilcrow/modules/jellyfish_renderer/GL/GLWindow.h"

namespace Jellyfish {
//-------------------
// * API SELECTION *
//-------------------
// enum eRenderAPI {E_GL, E_DX11, E_DXR, E_VKN, E_DX12};
// eRenderAPI g_API = E_GL; ///TODO: data-driven API choice

//-------------------
// * Testing Stub *
//-------------------
// basic linker test

class Derp {
public:
  void DoAThing();

private:
  std::string str = "Oh look a jellyfish!";
};

class RenderableObject {
  int stub;
};

class Renderer {
  void Draw() {}  // not used yet!!
};
}  // namespace Jellyfish