#pragma once
#include <string>
class Resource {
public:
  enum class IOType{ text, binary };
  Resource( const std::string &name );
  Resource( const Resource &resource );
  Resource( Resource &&resource );
  virtual ~Resource( );
    
  std::string Path( ) const;
  const std::string& Name( ) const;
  const std::string& Extension( ) const;
  std::string Filename( ) const;
  const std::string& Data( ) const;
  bool Save( IOType loadType = IOType::text ) const;
  bool Loaded( ) const;
  
  bool Load( IOType loadType = IOType::text );
  void Unload( );
  bool Reload( );

  virtual bool Reloadable( ) const = 0;
  virtual std::string Directory( ) const = 0;

protected:
  virtual bool LoadImpl( ) = 0;
  virtual void UnloadImpl( ) = 0;

  bool Read( IOType loadType = IOType::text );
private:
  bool m_loaded{ false };
  std::string m_name;
  std::string m_extension;
protected:
  std::string m_data;
};
