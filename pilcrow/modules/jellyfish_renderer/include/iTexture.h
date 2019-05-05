#pragma once

namespace Jellyfish
{
	class iTexture
	{
	public:
		// Add others as needed
		enum class TextureType
		{
			diffuse,
			specular,
			normalmap,
			cubemap,
			heightmap,
			lightmap,
			ambient,
			emissive,
			shininess,
			opacity,
			displacement,
			reflection,
			image
		}; //end enum TextureType

		virtual ~iTexture() {}
		
		virtual void Use(int TextureUnit = 0) const = 0;
		virtual TextureType Type() const = 0;
		virtual unsigned ID() const = 0;

		//Loading functions are derived from the Resource class in the derived class

	protected:
		int m_Width{ 0 }, m_Height{ 0 };
		int m_NumChannels{ 0 };
		TextureType m_Type{ TextureType::image };
	};//endclass iTexture
	
} //end namespace Jellyfish
