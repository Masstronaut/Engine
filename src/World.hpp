#pragma once
#include <string>
#include <vector>
#include <functional>

#include <slot_map.hpp>

#include "Entity.hpp"

class ComponentPoolBase {
public:
  virtual ~ComponentPoolBase( ) { }
};

template<typename Component>
class ComponentPool : public ComponentPoolBase {
  slot_map<Component> components;
};

class World {
public:
  World( const std::string &name )
    : m_Name( name ) {
  }

  template<typename T>
  T& GetComponent( EntityID entity ) {
    return GetComponentPool<T>( ).components[ entity ];
  }
  template<typename T>
  const T& GetComponent( EntityID entity ) const {
    return GetComponentPool<T>( ).components[ entity ];
  }

protected:
  template<typename T>
  ComponentPool<T>& GetComponentPool( ) {
    auto it{ m_Components.find( typeid( T ).hash_code( ) ) };
    if( it != m_Components.end( ) ) {
      return *reinterpret_cast< ComponentPool<T>* >( it->second.get( ) );
    } else {
      it = m_Components.emplace( typeid( T ).hash_code( ), std::make_unique<ComponentPoolBase>( new ComponentPool<T>( ) ) ).first;
      return *reinterpret_cast< ComponentPool<T>* >( it->second.get( ) );
    }
  }
private:

  std::string m_Name;
  std::vector < std::function<void( )>> m_Systems;
  std::unordered_map<std::size_t, std::unique_ptr<ComponentPoolBase>> m_Components;
};