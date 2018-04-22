#include <iostream>
#include <unordered_map>


#include "../../../Modules/Utils/include/Resource.hpp"
#include "../Include/Model.h"
#include "../Include/iMesh.h"
#include "../Include/iTexture.h"

#include "../Include/AssimpLoaders.hpp"

//TODO: fix
#include "../include/GLMesh.h"

namespace Jellyfish
{
	Model::Model(const std::string & path) : Resource(path)
	{
		Resource::Load(IOType::binary);
	}

	Model::Model(const Model & model)
		: Resource(model) {
		Load();
	}

	Model::~Model()
	{
	}

	void Model::Draw() const
	{
		for (auto& mesh : m_Meshes)
		{
			//mesh.Draw();
		}
	}

	bool Model::Reloadable() const
	{
		return false;
	}

	bool Model::LoadImpl() 
	{
		//use assimp loader
		//Model* loadedModel = Assimp_LoadModel(this->Path(), this->Name());
		//copy or something
		return false;
	}

	void Model::UnloadImpl()
	{

	}

	/*
	std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures( aiMaterial *mat, aiTextureType type ) {
	std::vector<std::shared_ptr<Texture>> textures;
	static std::unordered_map<std::string, std::shared_ptr<Texture>> loaded;

	for( unsigned int i = 0; i < mat->GetTextureCount( type ); ++i )
	{
	aiString str;
	mat->GetTexture( type, i, &str );

	//Texture texture;
	//texture.id = TextureFromFile( str.C_Str( ), directory );
	//texture.type = typeName;
	//texture.path = str;

	auto it{ loaded.find( str.C_Str( ) ) };

	if( it != loaded.end( ) )
	{
	textures.push_back( it->second );
	}
	else
	{
	auto res = loaded.emplace( str.C_Str( ), std::make_shared<Texture>( str.C_Str( ) ) );
	textures.push_back( res.first->second );
	if( type == aiTextureType_DIFFUSE )
	textures.back( )->Type( TextureType::diffuse );
	else if( type == aiTextureType_SPECULAR )
	textures.back( )->Type( TextureType::specular );
	}
	}
	return textures;
	}
	*/

}