#pragma once

//GL
#include <glad/include/glad.h>

//ours
#include <utils/include/Resource.hpp>
#include "../iTexture.h"


namespace Jellyfish
{
	class GLTexture : virtual public iTexture
	{
	public:
		GLTexture(const std::string &name);
		~GLTexture();
		GLTexture& Type(TextureType type);

		//Overriding iTexture:
		void Use(int TextureUnit = 0) const final;
		TextureType Type() const final;
		unsigned ID() const final;

		//Overriding Resource:
		virtual bool Reloadable() const final;
		virtual std::string Directory() const final;

	private:
		//Overriding Resource:
		bool LoadImpl() final;
		void UnloadImpl() final;


		GLint TextureFromData();
		GLuint m_GLuID;
		GLenum m_Format;
		
	}; // class GLTexture


} //namespace Jellyfish
