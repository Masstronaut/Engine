#include <glad/include/glad.h>
#include "../Include/GLMesh.h"

namespace Jellyfish
{
	void GLMesh::Draw() const
	{
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
		//
		//glActiveTexture(GL_TEXTURE0);
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
	}
}