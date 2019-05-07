#pragma once

//std
#include <string>
#include <vector>
#include "RenderResourceManager.h"

class iShader;
class iTexture;
namespace Jellyfish
{
	// need a way to store shader uniform settings
	struct ShaderSettings
	{
		//TODO
	};


	class Material
	{
	public:
		Material(int index = -1) //materials are a special case for loading where you might only have a unique index
		{
			//save the Material
			Render_Resource_Manager& rrm = Render_Resource_Manager::get();

			//use a -1 as an id not specified
			if(index == -1)
				m_ID = rrm.AddToManager(this);
			else
				m_ID = rrm.AddToManager(index, this);
		}

		//TODO:
		// stuff to create a shader network here

		// for now we will just have one shader per material
		// can still have multiple textures though, how do we 	

	private:
		unsigned int m_ID;
		std::string m_name;
		std::vector<unsigned int> m_shader_ids;
		std::vector<unsigned int> m_texture_ids;
	};
}