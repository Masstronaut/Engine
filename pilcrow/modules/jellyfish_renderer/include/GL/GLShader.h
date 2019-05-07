#pragma once

//GL
#include <glad/include/glad.h> //GLuint

//ours
#include <utils/include/Resource.hpp>

#include "../iShader.h"

namespace Jellyfish
{
	class GLShader : virtual public iShader
	{
	public:
		 GLShader(const std::string &name);
		 GLShader(GLShader && shader);
		~GLShader();

		//Overriding iShader:
		unsigned Type() const final;
		void Use() const final;
		bool Check() const final;

		//Overriding Resource:
		bool Reloadable() const final;
		std::string Directory() const final;

	private:
		//Overriding Resource:
		bool LoadImpl();
		void UnloadImpl();

		GLuint m_GLuID;
		unsigned m_GLShaderType;

	}; // class GLShader


} //namespace Jellyfish
