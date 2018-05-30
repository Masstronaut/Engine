#include <typeinfo>
#include <vector>

#include "../../include/Entity/Entity.hpp"
#include "../../include/World.hpp"
#include "../../include/entity/EntityRef.hpp"

Entity::Entity(World &world, EntityID id  )
  : m_World( world ) 
  , m_ID( id )
  , m_Name( "Nameless Entity" ) {
}
Entity::Entity( Entity &&rhs )
  : m_World(rhs.m_World)
  , m_Components( std::move( rhs.m_Components ) )
  , m_ID( rhs.m_ID )
  , m_Name( std::move( rhs.m_Name ) ) {
  
}
Entity& Entity::operator=(Entity &&rhs) {
  this->SafelyDisposeComponents();
  m_Components = std::move(rhs.m_Components);
  m_ID = rhs.m_ID;
  m_Name = std::move(rhs.m_Name);
  return *this;
}
Entity::~Entity( ) {
  this->SafelyDisposeComponents();
}


bool Entity::Has( std::type_index component_type ) const {
  return m_Components.count( component_type ) > 0;
}

void Entity::SafelyDisposeComponents(){
  for (auto &component : m_Components) {
    m_World.GetComponentPool(component.first)->Erase(component.second);
  }
  m_Components.clear();
}

EntityID Entity::Clone( World &world, Entity &entity ) const {
  for( const auto& comp : m_Components ) {
    entity.m_Components.emplace( m_World.GetComponentPool( comp.first )->Clone( comp.second, world ) );
  }
  world.Emit<EntitySpawnedEvent>( EntitySpawnedEvent( EntityRef( entity.ID( ), &world ) ) );
  return entity.ID( );
}

EntityID Entity::ID( ) const {
  return m_ID;
}

const std::string& Entity::Name( ) const {
  return m_Name;
}
Entity& Entity::Name( const std::string &name ) {
  m_Name = name;
  return *this;
}

EntityRef Entity::Clone( ) const {
  EntityRef clone{ m_World.Spawn( EntityRef( this->ID( ), &m_World ) ) };
  return clone;
}

void * Entity::Get( std::type_index component ) {
  return m_World.GetComponent( m_Components[component] ,component );
  return nullptr;
}
