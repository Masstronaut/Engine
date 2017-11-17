#pragma once

#include <unordered_map>
class Entity;
class ComponentHandle;
using EntityID = std::pair<unsigned, unsigned>;

class Entity {
public:
  Entity( );
  Entity( const Entity & );
  Entity( Entity && );
  ~Entity( );

  template<typename Component>
  bool Has( ) const;


  template<typename Component>
  Component& Get( );
  template<typename Component>
  const Component& Get( ) const;

  EntityID ID( ) const;
  
  const std::string& Name( ) const;
  Entity& Name( const std::string &name );
  
private:
  std::unordered_map<std::size_t, EntityID> m_Components;
  EntityID m_ID{ 0, 0 };
  std::string m_Name{"Nameless Entity"};


};