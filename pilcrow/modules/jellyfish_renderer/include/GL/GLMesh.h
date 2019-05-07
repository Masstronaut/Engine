#pragma once

//ours
#include "../iMesh.h"
#include "GLTexture.h"

namespace Jellyfish
{
	class GLMesh : public iMesh
	{
	public:
		
		GLMesh(const std::vector<Vertex>& Vertices, const std::vector<unsigned>& Indices);

		~GLMesh()
		{}

		void Load();
		void Draw() const;

	private:
		unsigned VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
	};
}