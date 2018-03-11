#pragma once
#include <functional>
#include <utility> // forward
#include "SystemTraits.hpp"
#include "WorldEvents.hpp"
class World;
class SystemBase {
public:
  virtual void AddSystem( World &world ) = 0;
  virtual bool HasEntities( ) const = 0;
  virtual bool HasVoidUpdate( ) const = 0;
  virtual bool HasDTUpdate( ) const = 0;
  virtual bool HasFixedUpdate( ) const = 0;
  virtual bool HasEditorUpdate( ) const = 0;
  virtual bool HasFrameStart( ) const = 0;
  virtual bool HasFrameEnd( ) const = 0;
  virtual bool HasPreProcess( ) const = 0;
  virtual bool HasProcess( ) const = 0;
  virtual bool IsParallelSystem( ) const = 0;
  virtual bool IsPureSystem( ) const = 0;
};

template<typename T>
class System : public SystemBase {
public:
  System( World &world, const std::string & name );
  virtual bool HasEntities( ) const final { return SystemTraits<T>::HasEntities; }
  virtual bool HasVoidUpdate( ) const final { return SystemTraits<T>::HasVoidUpdate; }
  virtual bool HasDTUpdate( ) const final { return SystemTraits<T>::HasDTUpdate; }
  virtual bool HasFixedUpdate( ) const final { return SystemTraits<T>::HasFixedUpdate; }
  virtual bool HasEditorUpdate( ) const final { return SystemTraits<T>::HasEditorUpdate; }
  virtual bool HasFrameStart( ) const final { return SystemTraits<T>::HasFrameStart; }
  virtual bool HasFrameEnd( ) const final { return SystemTraits<T>::HasFrameEnd; }
  virtual bool HasPreProcess( ) const final { return SystemTraits<T>::HasPreProcess; }
  virtual bool HasProcess( ) const final { return SystemTraits<T>::HasProcess; }

  virtual bool IsPureSystem( ) const final { return SystemTraits<T>::IsPureSystem; }
  virtual bool IsParallelSystem( ) const { return SystemTraits<T>::IsParallelSystem; }

private:
  virtual void AddSystem( World &world ) final;
  void RegisterEntities( World &world );
  void RegisterVoidUpdate( World &world );
  void RegisterDTUpdate( World &world );
  void RegisterFixedUpdate( World &world );
  void RegisterEditorUpdate( World &world );
  void RegisterParallelSystemProcess( World &world );
  void RegisterDtMember( World &world );
  T instance;
  std::string m_name;
};

#include "World.hpp"
template<typename T>
inline System<T>::System( World &world, const std::string & name )
  : m_name( name ) {
  AddSystem( world );
}
template<typename T>
inline void System<T>::AddSystem( World & world ) { 
  if constexpr( SystemTraits<T>::HasEntities ) {
    this->RegisterEntities( world );
  }
  if constexpr( SystemTraits<T>::HasVoidUpdate ) {
    this->RegisterVoidUpdate( world );
  }
  if constexpr ( SystemTraits<T>::HasDTUpdate ) {
    this->RegisterDTUpdate( world );
  }
  if constexpr( SystemTraits<T>::HasFixedUpdate ) {
    this->RegisterFixedUpdate( world );
  }
  if constexpr( SystemTraits<T>::HasEditorUpdate ) {
    this->RegisterEditorUpdate( world );
  }
  if constexpr( SystemTraits<T>::IsParallelSystem ) {
    this->RegisterParallelSystemProcess( world );
  }
  if constexpr( SystemTraits<T>::HasDtMember ) {
    this->RegisterDtMember( world );
  }
}

template<typename T>
inline void System<T>::RegisterEntities( World & world ) {
  world.RegisterEntitiesWith( instance.Entities );
}

template<typename T>
inline void System<T>::RegisterVoidUpdate( World & world ) {
  world.AddSystem( static_cast< void( T::* )( ) >( &T::Update ), instance, m_name );
}

template<typename T>
inline void System<T>::RegisterDTUpdate( World & world ) { 
  world.AddSystem( std::function<void(float)>([ this ]( float dt ) { instance.Update( dt ); }), m_name );
}

template<typename T>
inline void System<T>::RegisterFixedUpdate( World & world ) {
  world.AddSystem( std::function<void( float )>( [ &, time = 0.f ]( float dt ){
    time += dt; 
    //@@TODO: remove the hard-coded 1/60 and replace it with fixed update framerate
    if( time >= 1.f / 60.f ) {
      time -= dt;
      this->instance.FixedUpdate( );
    }
  }, m_name ) );
}

template<typename T>
inline void System<T>::RegisterEditorUpdate( World & world ) { 
  world.On<EditorUpdateEvent>( [ & ]( const EditorUpdateEvent & ) { instance.EditorUpdate( ); }, m_name );
}

#include "EntitiesWith.hpp"

template<typename T>
inline void System<T>::RegisterParallelSystemProcess( World &world ) {
  world.AddUpdater( m_name, std::move(
    [ &, Entities = EntitiesToProcess( &T::Process ), registered = false ]( float dt ) mutable {
    if( !registered ) {
      registered = true;
      world.RegisterEntitiesWith( Entities );
    }
    if constexpr( SystemTraits<T>::HasPreProcess ) {
      this->instance.PreProcess( );
    }
    for( auto entity : Entities ) { 
      entity.Invoke( [ & ]( auto&&... args ) {
        instance.Process( std::forward<decltype( args )>( args )... ); 
      });
    }
  } ) );
}

template<typename T>
inline void System<T>::RegisterDtMember( World & world ) { 
  world.On<UpdateEvent>( [ & ]( const UpdateEvent &ue ) { 
    instance.Dt = ue.Dt; 
  } );
}
