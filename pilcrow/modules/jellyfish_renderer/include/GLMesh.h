#pragma once

#include "iMesh.h"

namespace Jellyfish
{
	class GLMesh : public iMesh
	{
	public:
		GLMesh()
		{
			//undefined behavior .. really shouldn't get used
		}
		GLMesh(const std::vector<Vertex>& Vertices, const std::vector<unsigned>& Indices, std::vector<std::shared_ptr<iTexture>>& Textures) :
			iMesh (Vertices, Indices, Textures)
		{
			Load();
		}

		~GLMesh()
		{}

		void Draw() const;
		void Load();

	private:
		unsigned VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
	};
}