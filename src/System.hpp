#pragma once
#include <functional>
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

  virtual bool IsPureSystem( ) const final { return SystemTraits<T>::IsPureSystem; }

private:
  virtual void AddSystem( World &world ) final;
  void RegisterEntities( World &world );
  void RegisterVoidUpdate( World &world );
  void RegisterDTUpdate( World &world );
  void RegisterFixedUpdate( World &world );
  void RegisterEditorUpdate( World &world );
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
    RegisterEntities( world );
  }
  if constexpr( SystemTraits<T>::HasVoidUpdate ) {
    RegisterVoidUpdate( world );
  }
  if constexpr ( SystemTraits<T>::HasDTUpdate ) {
    RegisterDTUpdate( world );
  }
  if constexpr( SystemTraits<T>::HasFixedUpdate ) {
    RegisterFixedUpdate( world );
  }
  if constexpr( SystemTraits<T>::HasEditorUpdate ) {
    RegisterEditorUpdate( world );
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
  world.AddSystem( static_cast< void( T::* )( float ) >( &T::Update ), instance, m_name );
}

template<typename T>
inline void System<T>::RegisterFixedUpdate( World & world ) {
  world.AddSystem( [ &, time = 0.f ]( float dt ){ 
    time += dt; 
    //@@TODO: remove the hard-coded 1/60 and replace it with fixed update framerate
    if( time >= 1.f / 60.f ) {
      time -= dt;
      instance.FixedUpdate( );
    }
  }, m_name );
}

template<typename T>
inline void System<T>::RegisterEditorUpdate( World & world ) { 
  world.On<EditorUpdateEvent>( [ & ]( const EditorUpdateEvent & ) { instance.EditorUpdate( ); }, m_name );
}
