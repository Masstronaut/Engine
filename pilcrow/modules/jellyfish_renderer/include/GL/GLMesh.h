#pragma once

//ours
#include "../iMesh.h"
#include "GLTexture.h"

namespace Jellyfish
{
	class GLMesh : public iMesh
	{
	public:
		
		GLMesh(const std::vector<Vertex>& Vertices, const std::vector<unsigned>& Indices, std::vector<std::shared_ptr<GLTexture>>& Textures);

		~GLMesh()
		{}

		void Load();
		void Draw() const;

		void AssignShader(GLProgram& shader) override;

	private:
		unsigned VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
	};
}