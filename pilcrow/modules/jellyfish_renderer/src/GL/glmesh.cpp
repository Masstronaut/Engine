#include <iostream>
#include <glad/include/glad.h>
#include "../../include/GL/GLMesh.h"
#include "../../include/GL/GLTexture.h"

namespace Jellyfish
{
	void GLMesh::Draw() const
	{
		unsigned diffuse{ 1 }, specular{ 1 };
		
		for (unsigned i{ 0 }; i < m_Textures.size(); ++i) 
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string name;
			
			switch (m_Textures[i]->iTexture::Type())
			{
				case iTexture::TextureType::diffuse:
				{
					name = "diffuse";
					name += std::to_string(diffuse++);
					
					if (!m_shader->SetUniform(name, (int)i))
					{
						std::cout << "Error! Could not set shader uniform:" << name << std::endl;
					}
					else
					{
						glBindTexture(GL_TEXTURE_2D, m_Textures[i]->ID());
						glActiveTexture(GL_TEXTURE0);
					}

					break;
				}
				
			//case iTexture::TextureType::specular:
			//	name = "specular";
			//	name += std::to_string(specular++);
			//	break;
			}
		}
		
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
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(decltype(m_Indices)::value_type), m_Indices.data(), GL_STATIC_DRAW);

		//Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		//Color Attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_Normal));
		glEnableVertexAttribArray(1);
		
		//Texcoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_TexCoords));
		glEnableVertexAttribArray(2);

		//Unbind VAO
		glBindVertexArray(0);
	}
}