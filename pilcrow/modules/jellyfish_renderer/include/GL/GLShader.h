#pragma once

//GL
#include <glad/include/glad.h> //GLuint

//ours
#include <utils/include/Resource.hpp>

#include "../iShader.h"

namespace Jellyfish
{
	class GLShader : public iShader, public Resource
	{
	public:
		 GLShader(const std::string &name);
		 GLShader(GLShader && shader);
		~GLShader();

		//Overriding iShader:
		unsigned ID() const override;
		unsigned Type() const override;
		void Use() const override;
		bool Check() const override;

		//Overriding Resource:
		bool Reloadable() const override;
		std::string Directory() const override;

	private:
		//Overriding Resource:
		bool LoadImpl();
		void UnloadImpl();

		GLuint m_GLuID;
		unsigned m_GLShaderType;

	}; // class GLShader


} //namespace Jellyfish
