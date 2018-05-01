#include <string>
#include <glad/include/glad.h>
#include <stb_image/stb_image.h>
#include "../../include/GL/GLTexture.h"
#include "Utils/include/ResourceSettings.h"

namespace Jellyfish
{
	///WIP
	//GLint TextureFromFile(const char* path, std::string directory, bool gamma)
	//{
	//	//Generate texture ID and load texture data 
	//	std::string filename = std::string(path);
	//	filename = directory + '/' + filename;
	//	GLuint textureID;
	//	glGenTextures(1, &textureID);
	//	int width, height;
	//	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	//	// Assign texture to ID
	//	glBindTexture(GL_TEXTURE_2D, textureID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, gamma ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//
	//	// Parameters
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//	SOIL_free_image_data(image);
	//	return textureID;
	//}

}//end namespace Jellyfish