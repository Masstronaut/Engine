#pragma once
#include <tuple>
#include <string>
#include <typeindex>
#include "EntityID.hpp"
class World;
class Entity;

class EntityRef {
public:
  EntityRef( EntityID ID, World* world );
  EntityRef( const EntityRef& ) = default;
  EntityRef( EntityRef&& ) = default;
  ~EntityRef( ) = default;
  EntityRef& operator=( const EntityRef &rhs );

  template<typename Component>
  bool Has( ) const;
  template<typename Component, typename... Components>
  bool Has( ) const;

  bool Has( std::type_index component_type ) const;

  template<typename Component>
  Component& Get( );
  template<typename Component>
  const Component& Get( ) const;

  EntityID ID( ) const;
  
  const std::string& Name( ) const;
  EntityRef& Name( const std::string &name );
  
  bool operator==( const EntityRef &rhs ) const;

  EntityRef Clone( ) const;
  // World needs access in order to implement cloning
  friend class World;
protected:
  EntityID m_ID{ 0, 0 };
  World *m_World;
};
#include "Entity/EntityRef.inl"
