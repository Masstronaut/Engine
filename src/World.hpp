#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

#include <slot_map.hpp>
#include "EventArena.hpp"
#include "Entity.hpp"
#include "ComponentAggregate.hpp"
class ComponentPoolBase {
public:
  virtual ~ComponentPoolBase( ) { }
};

template<typename Component>
class ComponentPool : public ComponentPoolBase {
  slot_map<Component> components;
};

class World : public EventArena {
public:
  World( const std::string &name = "Nameless World" );

  template<typename T>
  T& GetComponent( EntityID entity );
  template<typename T>
  const T& GetComponent( EntityID entity ) const;

  Entity& GetEntity( EntityID ID );
  const Entity& GetEntity( EntityID ID ) const;

  // System styles:
  // PURE systems (NO singleton data)
  // Systems of the form (dt, Args&...) which are called once per matching entity.
  // Systems of the form (dt) which are called once per frame.
  // Full class systems
  // STATEFUL systems (require singleton data, maybe multiple entity lists)
  // Full class systems

  // Pure systems of the form (dt) which are called once per frame
  template<typename ReturnType>
  void AddSystem( ReturnType( *fn )( float ), const std::string& name = "Nameless System" );
  // Member functions that run once per update tick
  template<typename ReturnType, typename ClassName>
  void AddSystem( ReturnType( ClassName::*fn )( float ), ClassName& instance, const std::string&& name = "Nameless System" );
  // const Member functions that run once per update tick
  template<typename ReturnType, typename ClassName>
  void AddSystem( ReturnType( ClassName::*fn )( float ) const, const ClassName& instance, const std::string&& name = "Nameless System" );

  template<typename... Args>
  void RegisterEntitiesWith( EntitiesWith<Args...>& );

  template<typename T>
  void AddSystem( const std::string &name = "Nameless System" );

  bool operator==( const World &rhs );
  const std::string& Name( ) const;

protected:
  template<typename T>
  void AddPureSystem( const std::string & = "Nameless System" );
  template<typename T>
  void AddStatefulSystem( const std::string & = "Nameless System" );
  class Updater {
  public:
    Updater( const std::string&& Name, std::function<void( float )>&& Fn )
      : name( std::move( Name ) )
      , fn( std::move( Fn ) ) { }
    inline void operator()( float dt ) const { fn( dt ); }
    std::string name;
    std::function<void( float )> fn;
  };
  template<typename T>
  ComponentPool<T>& GetComponentPool( );
  template<typename... Args>
  ComponentAggregate& GetAggregate( );
  template<typename... Args>
  ComponentAggregate& GetAggregate( type_list<Args...> );
private:

  std::string m_Name;
  std::unordered_map<std::size_t, std::unique_ptr<ComponentPoolBase>> m_Components;
  slot_map<Entity> m_Entities;
  std::vector<ComponentAggregate> m_Aggregates;
  std::vector<Updater> m_Systems;
};

template<typename T>
T& World::GetComponent( EntityID entity ) {
  return GetComponentPool<T>( ).components[ entity ];
}
template<typename T>
const T& World::GetComponent( EntityID entity ) const {
  return GetComponentPool<T>( ).components[ entity ];
}

template<typename T>
ComponentPool<T>& World::GetComponentPool( ) {
  auto it{ m_Components.find( std::type_index( typeid( T ) ) ) };
  if( it != m_Components.end( ) ) {
    return *reinterpret_cast< ComponentPool<T>* >( it->second.get( ) );
  } else {
    it = m_Components.emplace( std::type_index( typeid( T ) ), std::make_unique<ComponentPoolBase>( new ComponentPool<T>( ) ) ).first;
    return *reinterpret_cast< ComponentPool<T>* >( it->second.get( ) );
  }
}


template<typename... Args>
ComponentAggregate& World::GetAggregate( ) {
  std::vector<std::type_index> hashes{ std::type_index( typeid( Args ) )... };
  std::sort( std::begin( hashes ), std::end( hashes ) );
  for( auto& aggregate : m_Aggregates ) {
    if( aggregate.Matches( hashes ) ) {
      return aggregate;
    }
  }
  m_Aggregates.emplace_back( type_list<Args...>{} );
  return m_Aggregates.back( );
}

template<typename... Args>
ComponentAggregate& World::GetAggregate( type_list<Args...> ) {
  return this->GetAggregate<Args...>( );
}
template<typename ReturnType>
void World::AddSystem( ReturnType( *fn )( float ), const std::string& name ) {
  m_Systems.emplace_back( Updater{ std::move( name ),
                          [ fn ]( float dt ) {
                            fn( dt );
                          }
  } );
}
// Member functions that run once per update tick
template<typename ReturnType, typename ClassName>
void World::AddSystem( ReturnType( ClassName::*fn )( float ), ClassName& instance, const std::string&& name ) {
  m_Systems.emplace_back( Updater{ std::move( name ),
                           [ fn, &instance ]( float dt ) {
                             ( instance.*fn )( dt );
                           }
  } );
}
// const Member functions that run once per update tick
template<typename ReturnType, typename ClassName>
void World::AddSystem( ReturnType( ClassName::*fn )( float ) const, const ClassName& instance, const std::string&& name ) {
  m_System.emplace_back( Updater{ std::move( name ),
                           [ fn, &instance ]( float dt ) {
                             ( instance.*fn )( dt );
                           }
  } );
}

template<typename T>
void World::AddSystem( const std::string &name ) {
  if constexpr( std::is_empty_v<T> ) {
    // PURE system
    AddPureSystem<T>( name );
  }
  if constexpr ( !std::is_empty_v<T> ) {
    // STATEFUL system
    AddStatefulSystem<T>( name );
  }
}



template<typename... Args>
void World::RegisterEntitiesWith( EntitiesWith<Args...> &ew ) {
  this->GetAggregate<Args...>( ).AddEntityList( ew );
}


#include "Detectors.hpp"
template<typename T>
void World::AddPureSystem( const std::string & name ) {
  static_assert( 0, "implementation of World::AddPureSystem does not yet exist." );
}
template<typename T>
void World::AddStatefulSystem( const std::string & name ) {
  if constexpr( has_Entities_v<T> ) {
    ComponentAggregate& agg{ GetAggregate<typename T::Entities>( ) };
  }
  //static_assert( 0, "Implementation of World::AddStatefulSystem does not yet exist." );
}