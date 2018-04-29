#pragma once
#include <utils/include/Resource.hpp>

//TODO: Add others
enum class TextureType { image, diffuse, specular, normalmap, cubemap };

namespace Jellyfish
{
	class iTexture
	{
	public:
		virtual ~iTexture() {}
		unsigned ID() const { return m_ID; };

		virtual bool Reloadable() const = 0;
		virtual std::string Directory() const
		{}
		virtual void Use(int TextureUnit = 0) const = 0;
		TextureType Type() const { return m_Type; }
		
	protected:
		virtual bool LoadImpl() = 0;
		virtual void UnloadImpl() = 0;
	
	private:
		unsigned m_ID{ 0 };
		int m_Width{ 0 }, m_Height{ 0 };
		int m_NumChannels{ 0 };
		TextureType m_Type{ TextureType::image };
    };
}