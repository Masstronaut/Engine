#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <tuple>
#include <typeinfo>
#include <typeindex>
#include <slot_map.hpp>

#include "EventArena.hpp"
//#include "Entity.hpp"
#include "Component/ComponentAggregate.hpp"
class ArchetypeRef;
class SystemBase;
class EntityRef;
#include "Component/ComponentPool.hpp"

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

  // systems that have already been configured by the engine internally
  void AddSystem( std::function<void( float )> &&fn, std::string &name );

  // Pure systems of the form (dt) which are called once per frame
  template<typename ReturnType>
  void AddSystem( ReturnType( *fn )( float ), const std::string& name = "Nameless System" );
  template<typename ReturnType>
  void AddSystem( ReturnType( *fn )( void ), const std::string& name = "Nameless System" );
  // Lambda functions that are called once per frame
  template<typename Predicate>
  std::enable_if_t<HasOperatorFnCall_v<Predicate>>
    AddSystem( Predicate&& pred, const std::string &name );
  // Member functions that run once per update tick
  template<typename ReturnType, typename ClassName>
  void AddSystem( ReturnType( ClassName::*fn )( float ), ClassName& instance, const std::string& name = "Nameless System" );
  template<typename ReturnType, typename ClassName>
  void AddSystem( ReturnType( ClassName::*fn )( void ), ClassName& instance, const std::string& name = "Nameless System" );

  // const Member functions that run once per update tick
  template<typename ReturnType, typename ClassName>
  void AddSystem( ReturnType( ClassName::*fn )( float ) const, const ClassName& instance, const std::string& name = "Nameless System" );
  template<typename ReturnType, typename ClassName>
  void AddSystem( ReturnType( ClassName::*fn )( void ) const, const ClassName& instance, const std::string& name = "Nameless System" );

  // whole class systems
  template<typename T>
  void AddSystem( const std::string &name = "Nameless System" );
  
  void AddUpdater( const std::string& Name, std::function<void( float )>&& Fn ) {
    m_Updaters.emplace_back( Name, Fn );
  }

  template<typename... Args>
  ComponentAggregate& GetAggregate( );

  template<typename... Args>
  void RegisterEntitiesWith( EntitiesWith<Args...> &ew );

  void Update( float dt );

  bool operator==( const World &rhs );
  const std::string& Name( ) const;

protected:
  template<typename Component>
  friend class ComponentPool;
  friend class Simulation;
  friend class Entity;
  EntityRef CreateEntity( const std::string &name );
  template<typename T>
  void AddPureSystem( const std::string & = "Nameless Pure System" );
  template<typename T>
  void AddStatefulSystem( const std::string & = "Nameless Stateful System" );
  template<typename T>
  void AddParallelSystem( const std::string & = "Nameless Parallel System" );
  class Updater {
  public:
    Updater( const std::string& Name, std::function<void( float )>& Fn )
      : name( Name )
      , fn( Fn ) { }
    inline void operator()( float dt ) const { fn( dt ); }
    std::string name;
    std::function<void( float )> fn;
  };
  template<typename T>
  ComponentPool<T>& GetComponentPool( );
  ComponentPoolBase* GetComponentPool( std::type_index Component );
  template<typename... Args>
  ComponentAggregate& GetAggregate( type_list<Args...> );
private:
  std::string m_Name;
  std::unordered_map<std::type_index, std::unique_ptr<ComponentPoolBase>> m_Components;
  slot_map<Entity> m_Entities;
  std::vector<ComponentAggregate> m_Aggregates;
  std::vector<Updater> m_Updaters;
  std::vector<std::unique_ptr<SystemBase>> m_Systems;
};
#include "World.inl"
