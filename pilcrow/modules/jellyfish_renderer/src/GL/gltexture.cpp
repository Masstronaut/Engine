#pragma once

//std
#include <string> //std::string
#include <iostream> //cout

//stb image
#include <stb_image/stb_image.h> //image data loader

//ours
#include "../../include/GL/GLTexture.h"
#include "Utils/include/ResourceSettings.h"

namespace Jellyfish
{
	GLTexture::GLTexture(const std::string &name) : Resource(name)
	{
		this->Load(IOType::binary);
	}

	GLTexture::~GLTexture()
	{
		this->Unload();
	}

	void GLTexture::Use(int TextureUnit) const
	{
		glActiveTexture(GL_TEXTURE0 + TextureUnit);
		glBindTexture(GL_TEXTURE_2D, this->ID());
	}

	iTexture::TextureType GLTexture::Type() const
	{
		return m_Type;
	}

	unsigned GLTexture::ID() const
	{
		return m_GLuID;
	};

	bool GLTexture::Reloadable() const
	{
		return true;
	}

	std::string GLTexture::Directory() const
	{
		return std::string(g_ResourcePath) + "Textures/";
	}

	GLTexture& GLTexture::Type(iTexture::TextureType type)
	{
		m_Type = type;
		return *this;
	}

	bool GLTexture::LoadImpl()
	{
		return TextureFromData();
	}

	void GLTexture::UnloadImpl()
	{
		glDeleteTextures(1, &m_GLuID);
		m_Width = m_Height = m_NumChannels = m_GLuID = 0;
	}

	GLint GLTexture::TextureFromData()
	{
		unsigned char* imgData{ stbi_load_from_memory((const unsigned char*)this->Data().c_str(),
			static_cast<int>(this->Data().size()),
			&m_Width,
			&m_Height,
			&m_NumChannels,
			0
		) };//end imgData allocation

		if (imgData)
		{
			std::cout << "Texture read: " << Filename() << std::endl;

			if (m_NumChannels == 4)
			{
				m_Format = GL_RGBA;
			}
			else if (m_NumChannels == 3)
			{
				m_Format = GL_RGB;
			}
			else if (m_NumChannels == 1)
			{
				m_Format = GL_RED;
			}

			//Get a texture ID and assign it
			glGenTextures(1, &m_GLuID);
			glBindTexture(GL_TEXTURE_2D, m_GLuID);

			//Set parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			//Set filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//store it and generate mipmaps
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, imgData);
			glGenerateMipmap(GL_TEXTURE_2D);

			//free memory and unbind
			stbi_image_free(imgData);
			glBindTexture(GL_TEXTURE_2D, 0);

			std::cout << "Texture was loaded successfully: " << Filename() << std::endl;

			return true;
		} //endif
		
		std::cout << "Failed to read texture data: " << this->Directory() << this->Filename() << std::endl;
		return false;
	}

}//end namespace Jellyfish