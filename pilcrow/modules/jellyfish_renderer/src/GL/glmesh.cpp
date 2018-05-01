#include <glad/include/glad.h>
#include "../../include/GL/GLMesh.h"

namespace Jellyfish
{
	void GLMesh::Draw() const
	{
		//TODO: textures

		//unsigned diffuse{ 1 }, specular{ 1 };
		//
		//for (unsigned i{ 0 }; i < textures.size(); ++i) 
		//{
		//	glActiveTexture(GL_TEXTURE0 + i);
		//
		//	std::string name;
		//	switch (textures[i]->Type()) 
		//	{
		//	case TextureType::diffuse:
		//		name = "diffuse";
		//		name += std::to_string(diffuse++);
		//		break;
		//	case TextureType::specular:
		//		name = "specular";
		//		name += std::to_string(specular++);
		//		break;
		//	}
		//	
		//	if (!shader.SetUniform(name, (int)i)) 
		//	{
		//		//__debugbreak( );
		//	}
		//
		//	glBindTexture(GL_TEXTURE_2D, textures[i]->ID());
		//}
		
		//glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, (int)m_Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void GLMesh::Load()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, m_Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, m_TexCoords));


		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(decltype(m_Indices)::value_type), m_Indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}