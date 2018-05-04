#pragma once
namespace Jellyfish
{
	class iTexture
	{
	public:
		// Add others as needed
		enum class TextureType
		{
			image,
			diffuse,
			specular,
			normalmap,
			cubemap,
			heightmap,
			lightmap
		}; //end enum TextureType

		virtual ~iTexture() {}
		
		virtual void Use(int TextureUnit = 0) const = 0;

		TextureType Type() const 
		{ return m_Type; }

		unsigned ID() const 
		{ return m_ID; };

		//Loading functions are derived from the Resource class in the derived class

	protected:
		unsigned m_ID{ 0 };
		int m_Width{ 0 }, m_Height{ 0 };
		int m_NumChannels{ 0 };
		TextureType m_Type{ TextureType::image };
	};//endclass iTexture
	
} //end namespace Jellyfish
