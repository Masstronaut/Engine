#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <tuple>
#include <typeindex>
#include <slot_map.hpp>

#include "EventArena.hpp"
#include "Entity.hpp"
#include "ComponentAggregate.hpp"
class ArchetypeRef;
class World;
class ComponentPoolBase {
public:
  ComponentPoolBase( ) = default;
  ComponentPoolBase( const ComponentPoolBase& ) = delete;
  virtual void* Get( EntityID ID ) = 0;
  virtual EntityID Clone( EntityID ID ) = 0;
  virtual std::pair<std::type_index, EntityID> Clone( EntityID ID, World& world ) = 0;
  virtual ~ComponentPoolBase( ) { }
};

template<typename Component>
class ComponentPool : public ComponentPoolBase {
public:
  ComponentPool( ) = default;
  ~ComponentPool( ) = default;
  virtual void* Get( EntityID ID ) final {
    return &components[ ID ];
  }
  // @@TODO: On Clone() all ComponentAggregates holding
  // this type of component need to re-fetch pointers if reallocation occurred.
  // The easiest way to do this is probably an event triggering the re-fetch.
  // This could be emitted by the pool on the world, which aggregates could listen for.
  // "OnPointerInvalidation" event or something. Include the type index so they can early out
  // if they don't care about that kind of component.
  virtual EntityID Clone( EntityID ID ) final;
  virtual std::pair<std::type_index, EntityID> Clone( EntityID ID, World &world ) final;
  slot_map<Component> components;
};

class World : public EventArena {
public:
  World( const std::string &name = "Nameless World" );

  template<typename T>
  T& GetComponent( EntityID entity );
  template<typename T>
  const T& GetComponent( EntityID entity ) const;
  void* GetComponent( EntityID entity, std::type_index ComponentType );

  Entity& GetEntity( EntityID ID );
  const Entity& GetEntity( EntityID ID ) const;
  EntityRef Spawn( EntityRef archetype );
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
  template<typename Component>
  friend class ComponentPool;
  friend class Simulation;
  friend class Entity;
  EntityRef CreateEntity( const std::string &name );
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
  ComponentPoolBase* GetComponentPool( std::type_index Component );
  template<typename... Args>
  ComponentAggregate& GetAggregate( );
  template<typename... Args>
  ComponentAggregate& GetAggregate( type_list<Args...> );
private:
  std::string m_Name;
  std::unordered_map<std::type_index, std::unique_ptr<ComponentPoolBase>> m_Components;
  slot_map<Entity> m_Entities;
  std::vector<ComponentAggregate> m_Aggregates;
  std::vector<Updater> m_Systems;
};

template<typename T>
T& World::GetComponent( EntityID entity ) {
  return GetComponentPool<std::decay_t<T>>( ).components[ entity ];
}
template<typename T>
const T& World::GetComponent( EntityID entity ) const {
  return GetComponentPool<std::decay_t<T>>( ).components[ entity ];
}

template<typename T>
ComponentPool<T>& World::GetComponentPool( ) {
  auto it{ m_Components.find( std::type_index( typeid( T ) ) ) };
  if( it != m_Components.end( ) ) {
    return *reinterpret_cast< ComponentPool<T>* >( it->second.get( ) );
  } else {
    it = m_Components.emplace( std::type_index( typeid( T ) ),  new ComponentPool<T>( ) ).first;
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
  if constexpr( HasEntities_v<T> ) {
    ComponentAggregate& agg{ GetAggregate<decltype(T::Entities)>( ) };
  }
  //static_assert( 0, "Implementation of World::AddStatefulSystem does not yet exist." );
}


// @@TODO: On Clone() all ComponentAggregates holding
// this type of component need to re-fetch pointers if reallocation occurred.
// The easiest way to do this is probably an event triggering the re-fetch.
// This could be emitted by the pool on the world, which aggregates could listen for.
// "OnPointerInvalidation" event or something. Include the type index so they can early out
// if they don't care about that kind of component.
template<typename Component>
inline EntityID ComponentPool<Component>::Clone( EntityID ID ) {
  return components.insert( components[ ID ] );
}

template<typename Component>
std::pair<std::type_index, EntityID> ComponentPool<Component>::Clone( EntityID ID, World &world ) {
  return std::make_pair( std::type_index( typeid( Component ) ),
                         world.GetComponentPool<Component>( ).components.insert( this->components[ ID ] ) );
}
