#include "World.hpp"
#include "Simulation.hpp"
#include "WorldEvents.hpp"


World::World( const std::string &name )
  : m_Name( name ) { 
  this->On<EntitySpawnedEvent>( [ & ]( const EntitySpawnedEvent &event ) {
    for( auto &agg : m_Aggregates ) agg.OnEntityCreated( event.entity );
  } );
  this->On<EntityDeathEvent>( [ & ]( const EntityDeathEvent &event ) {
    for( auto &agg : m_Aggregates ) agg.OnEntityDestroyed( event.entity );
  } );
}


void * World::GetComponent( EntityID component, std::type_index ComponentType ) {
  auto it{ m_Components.find( ComponentType ) };
  if( it != m_Components.end( ) ) {
    return it->second->Get( component );
  }
  return nullptr;
}

Entity& World::GetEntity( EntityID ID ) {
  return m_Entities[ ID ];
}
const Entity& World::GetEntity( EntityID ID ) const {
  return m_Entities[ ID ];
}
EntityRef World::Spawn( EntityRef archetype ) {
  EntityRef ent{ this->CreateEntity( archetype.Name( ) ) };
  archetype.m_World->m_Entities[ archetype.ID( ) ].Clone( *this, m_Entities[ ent.ID( ) ] );
  return ent;
}

void World::AddSystem( std::function<void( float )>&& fn, std::string &name ) {
  m_Updaters.emplace_back( name, fn );
}

const std::string& World::Name( ) const {
  return m_Name;
}

EntityRef World::CreateEntity( const std::string & name ) {
  auto ID{ m_Entities.emplace( *this ) };
  m_Entities[ ID ].Name(name).m_ID = ID;
  return EntityRef( ID, this );
}

ComponentPoolBase * World::GetComponentPool( std::type_index Component ) {
  auto it{ m_Components.find( Component ) };
  if( it != m_Components.end( ) ) {
    return it->second.get( );
  }
  return nullptr;
}

void World::Update( float dt ) {
  this->Emit( UpdateEvent{ dt } );
  for( auto &updater : m_Updaters ) updater( dt );
}

bool World::operator==( const World &rhs ) {
  return ( this == &rhs );
}

