#pragma once

//-------------------------------------------------------------
// *** MAIN ENTRY POINT FOR THE JELLYFISH RENDERER LIBRARY ***
//-------------------------------------------------------------

//std
#include <string>

//Core
#include "iWindow.h" //Windowing
#include "Model.h" //3D Model support
#include "Camera.h" //3D ortho/persp Camera support
#include "RenderResourceManager.h" //Asset/Resource Management

//GL Implementations
#include "GL/GLMesh.h"
#include "GL/GLProgram.h" //includes GLShader.h
#include "GL/GLText.h"
#include "GL/GLTexture.h"
#include "GL/GLWindow.h"

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

	class Jellyfish_Renderer
	{
		Jellyfish_Renderer() 
		{

		}

		
		//things


		void Render() {} //not used yet!!
	};

	//PLAN:
	/*
	0. Fix compile errors on types and render a primitive from data.
	1. Create a basic singleton asset manager class that has a map for each type of asset we need (for now)
	2. Load assets into the asset manager and assign them an id'
	3. When we load an asset, check to see if it exists in the asset manager before loading, if it exists, just get the ID
		3a. check if it exists by filename for now, we can probably hash the filename into a unique id in the future
		3b. store the loaded asset id and loaded stringname/filename
		3c. be able to search and retrieve the object by ID
		3d. some objects will want to reference the same asset, we'll give these objects their own transforms, etc, but 
			there's no need to store multiple instances of an asset archetype in memory, essentially just mods to the archtype
			and which resource to be using
		3e. Current Typs:  Model/Mesh, Texture, Shader/Material
		3f. A model loader will have to call assimp's scene loader, the model will load meshes, 
		    and the meshes will load materials, which will load textures
		3g. The model load function will call the mesh function, the mesh function will call the material function.. and so on..
		3h. Each time this happens, the owning object will need to get some form of the child ones
		3i. Model contains  a head mesh ptr,  Mesh contains a material ptr or vector of material ptrs,  Material
	    3j. Shaders and Textures ...  Each will have their own loader, and shoudl only exist in memory once, 
		    but be able to be called with different uniform settings
	    3k. Material shaders should exist on the mesh, i.e. a mesh will have ID(s) material(s) being used
	    3l. A Material can use multiple shaders and textures, thus, the material will store the ID(s) of the shaders and textures.
	    3m. Eventually, we'll want to feed shaders and applied textures into each other, so, the material will need to be 
		    able to handle a kind of node-graph of shaders(with texture and uniform parameter inputs)
	4.  When we Render an asset (i.e. a model) that asset will lookup the resources it needs in the asset manager in its render function
		4a. Renderable Models and Meshes already call their own draw functions, so its the shader/texture stuff that needs adjustment
		4b. Need to create a Material Class that the mesh can call on the render with which holds those IDs
	5.  Jellyfish needs a Renderer class that will handle more of this stuff internally, i.e. moving a lot of the 
	    functionality and logic out of RenderSystem
		5a. The Renderer, when initialized, will create its Asset Manager to load and manage resources
		5b. In the External Game code, Initializing a Renderable with a filename or archetype containing a filename will use 
		    the component to tell the renderer what to load
		5c. The renderer class code should stay very high level and abstracted
		5d. Switching Render Modes is a function of the Renderer Class -- Will likely need to use the messaging system.
	6. Threading.  Once all of the above works, we'll try to thread the asset loading
		6a. Start with just the file parsing, reading it into data, and pass that data the main thread
		6b. Each mesh could be threaded as long as the structure is preserved...  
			ideas: save a traversal record for re-inserting, create the structure with placeholder meshes and match id's later, can combine these
		6c. meshes will spawn threads that load materials, which spawn threads that load shaders and textures
			- if we generate a unique id for the material, then we can just store the id on the mesh fro grabbing later
			- Since shaders/textures are the only asset loading parts, we can set up a material with an ID first, then send that ID back to the
			  Mesh, while the Material spawns threads for the shaders/textures and gives those threads the material ID to assign themselves to.
		6d. We'll need to be careful about checking for null resources since objects will want to use them before they've actually loaded.
		    - Fancy stuff: if we encounter this, we should fade in the resource with an interpolated opacity afterward, or do that grainy fade thing




	
	*/
}