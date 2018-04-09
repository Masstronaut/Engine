#pragma once
#include <Utils/include/Resource.hpp>


// @@TODO: Consider moving to a "Material" type
enum class TextureType { image, diffuse, specular };

class Texture : public Resource {
public:
  Texture( const std::string &name );
  ~Texture( );
  unsigned ID( ) const;

  virtual bool Reloadable( ) const override;
  virtual std::string Directory( ) const override;
  void Use( int TextureUnit = 0 ) const;
  TextureType Type( ) const;
  Texture& Type( TextureType type );
protected:
  virtual bool LoadImpl( );
  virtual void UnloadImpl( );
private:
  unsigned m_TextureID{ 0 };
  int m_Width{ 0 }, m_Height{ 0 };
  int m_NumChannels{ 0 };
  TextureType m_Type{ TextureType::image };
};
