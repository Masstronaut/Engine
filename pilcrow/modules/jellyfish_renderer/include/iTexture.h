#pragma once
//#include <assimp/Importer.hpp>
#include <assimp/scene.h>
//#include <assimp/postprocess.h>

namespace Jellyfish
{
	class iTexture : virtual public Resource
	{
	public:
		// Add others as needed
		// these are set to match the values of those in assimp so we can construct from those
		enum class TextureType
		{
			diffuse = aiTextureType_DIFFUSE,	          //0x1
			specular = aiTextureType_SPECULAR,	          //0x2
			ambient = aiTextureType_AMBIENT,	          //0x3
			emissive = aiTextureType_EMISSIVE,	          //0x4
			heightmap = aiTextureType_HEIGHT,	          //0x5
			normalmap = aiTextureType_NORMALS,	          //0x6
			shininess = aiTextureType_SHININESS,          //0x7
			opacity = aiTextureType_OPACITY,              //0x8
			displacement = aiTextureType_DISPLACEMENT,    //0x9
			lightmap = aiTextureType_LIGHTMAP,            //0xA
			reflection = aiTextureType_REFLECTION,        //0xB
			image = aiTextureType_UNKNOWN,                //0xC
			cubemap = 0xD                                 //0xD
		}; //end enum TextureType	


		iTexture()
		{
			//this should never get called directly
		}
		virtual ~iTexture() {}		
		virtual void Use(int TextureUnit = 0) const = 0;
		virtual TextureType Type() const = 0;
		virtual unsigned ID() const = 0;

		//inherited
		virtual bool Reloadable() const = 0;
		virtual std::string Directory() const = 0;
		virtual bool LoadImpl() = 0;
		virtual void UnloadImpl() = 0;

	protected:
		int m_Width{ 0 }, m_Height{ 0 };
		int m_NumChannels{ 0 };
		TextureType m_Type{ TextureType::image };
	};//endclass iTexture
	
} //end namespace Jellyfish
