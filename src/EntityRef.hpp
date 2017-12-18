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

class ArchetypeRef : public EntityRef {
public:
  ArchetypeRef( const ArchetypeRef& ) = default;
  ArchetypeRef( ) = delete;
  using EntityRef::EntityRef;
  using EntityRef::operator==;
  
  template<typename Component, typename... Args>
  Component& Add( Args&&... args ) {
    return m_World->GetEntity( m_ID ).Add<Component>( std::forward<Args>( args )... );
  }
  template<typename Component>
  void Remove( ) {
    m_World->GetEntity( m_ID ).Remove<Component>( );
  }
  friend class Simulation;
private:
  ArchetypeRef( EntityRef er )
    : EntityRef( er ) { }
};


template<typename Component>
bool EntityRef::Has( ) const {
  return Has( std::type_index( typeid( Component ) ) );
}
template<typename Component, typename... Components>
bool EntityRef::Has( ) const {
  return Has<Component>( ) && Has<Components...>( );
}

template<typename Component>
inline Component& EntityRef::Get( ) {
  return m_World->GetEntity( m_ID ).Get<Component>( );
}

template<typename Component>
inline const Component& EntityRef::Get( ) const {
  return m_World->GetEntity( m_ID ).Get<Component>( );
}

template<typename... Args>
class ConstrainedEntityRef {
public:
  ConstrainedEntityRef( const EntityRef& ConstrainedEntity );
  ConstrainedEntityRef( const ConstrainedEntityRef& ) = default;
  ~ConstrainedEntityRef( ) = default;

  template<typename Component, typename = std::enable_if_t<!std::is_const_v<Component>>>
  Component& Get( ) { return this->GetMutable<Component, Args...>( ); }

  template<typename Component, typename = std::enable_if_t<std::is_const_v<Component>>>
  const Component& Get( ) const { return this->GetImmutable<Component, Args...>( ); }
private:
  EntityRef m_Self;

  template<typename Component, typename T1>
  Component& GetMutable( );
  template<typename Component, typename T1, typename T2, typename... FnArgs>
  Component& GetMutable( );
  template<typename Component, typename T1>
  const Component& GetImmutable( ) const;
  template<typename Component, typename T1, typename T2, typename... FnArgs>
  const Component& GetImmutable( ) const;
};

template<typename... Args>
ConstrainedEntityRef<Args...>::ConstrainedEntityRef( const EntityRef& ConstrainedEntity ) 
  : m_Self( ConstrainedEntity ) {
}

template<typename... Args>
  template<typename Component, typename T1>
  Component& ConstrainedEntityRef<Args...>::GetMutable( ) {
    static_assert( !std::is_const_v<Component>, "This shouldn't happen. Please send a bug report to Allan." );
    static_assert( !std::is_reference_v<Component>,
                   "References cannot be components. Try removing the reference from your component list." );
    static_assert( std::is_same_v<Component, std::remove_const_t<T1>>,
                   "Accessing components not listed as required is not allowed." );
    static_assert( !std::is_const_v<T1>,
                   "Error: Cannot get non-const access to a component required as const." );
    if constexpr ( std::is_same_v<Component, T1> ) {
      // at this point the component is the right type, and can be given to the user:
      return m_Self.Get<Component>( );
    } else {
      static_assert( 0, "An error has occurred in accessing a component from an entity with it." );
    }
  }
  template<typename... Args>
  template<typename Component, typename T1, typename T2, typename... FnArgs>
  Component& ConstrainedEntityRef<Args...>::GetMutable( ) {
    /*
    static_assert( !std::is_const_v<Component>, "This shouldn't happen. Please send a bug report to Allan." );
    static_assert( !std::is_reference_v<Component>,
                   "References cannot be components. Try removing the reference from your component list." );
    if constexpr( std::is_same_v<Component, std::remove_const_t<T1>> ) {
      static_assert( !std::is_const_v<T1>,
                     "Error: Trying to get a non-const reference to a required component marked as const." );
      static_assert( std::is_same_v<Component, T1>,
                     "An unknown error has occurred. Please submit a bug report with your system source code." );
      return m_Self.Get<Component>( );
    }
    */
    if constexpr( std::is_same_v<Component, T1> ) {
      return m_Self.Get<Component>( );
    } else if constexpr( sizeof...( FnArgs ) ) {
      return this->GetMutable<Component, T2, FnArgs...>( );
    } else {
      return this->GetMutable<Component, T2>( );
    }
  }

  template<typename... Args>
  template<typename Component, typename T1>
  const Component& ConstrainedEntityRef<Args...>::GetImmutable( ) const {
    /*
    static_assert( std::is_const_v<Component>, "This shouldn't happen. Please send a bug report to Allan." );
    static_assert( !std::is_reference_v<Component>,
                   "References cannot be components. Try removing the reference from your component list." );
    static_assert( std::is_same_v<std::decay_t<Component>, std::decay_t<T1>>,
                   "Accessing components not listed as required is not allowed." );
    static_assert( std::is_const_v<T1>,
                   "Error: Cannot get non-const access to a component required as const." );
    static_assert( !std::is_reference_v<T1>, "Error: Reference types may not be used as components." );
    */
    if constexpr ( std::is_same_v<Component, T1> ) {
      // at this point the component is the right type, and can be given to the user:
      return m_Self.Get<Component>( );
    }
    else {
      static_assert( 0, "An error has occurred in accessing a component from an entity with it." );
    }
  }
  template<typename... Args>
  template<typename Component, typename T1, typename T2, typename... FnArgs>
  const Component& ConstrainedEntityRef<Args...>::GetImmutable( ) const {
    /*
    static_assert( std::is_const_v<Component>, "This shouldn't happen. Please send a bug report to Allan." );
    static_assert( !std::is_reference_v<Component>,
                   "References cannot be components. Try removing the reference from your component list." );
    */
    if constexpr( std::is_same_v<Component, T1> ) {
      return m_Self.Get<Component>( );
    }
    else if constexpr(sizeof...(FnArgs)) {
      return this->GetImmutable<Component, T2, FnArgs...>( );
    } else {
      return this->GetImmutable<Component, T2>( );
    }
  }


