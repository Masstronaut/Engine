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
  virtual bool HasDtUpdate( ) const = 0;
  virtual bool HasFixedUpdate( ) const = 0;
  virtual bool HasEditorUpdate( ) const = 0;
  virtual bool HasFrameStart( ) const = 0;
  virtual bool HasFrameEnd( ) const = 0;
  virtual bool HasPreProcess( ) const = 0;
  virtual bool HasProcess( ) const = 0;
  virtual bool IsParallelSystem( ) const = 0;
  virtual bool IsPureSystem( ) const = 0;

  virtual void OnUpdate( float Dt ) = 0;
};

namespace detail {

template<typename U>
auto EntitiesToProcess( const U&, typename std::enable_if_t<SystemTraits<U>::HasProcess>* = nullptr ) { return EntitiesToProcessHelper( &U::Process ); }
template<typename U>
bool EntitiesToProcess( const U&, typename std::enable_if_t<!SystemTraits<U>::HasProcess>* = nullptr ) { }
template<typename ReturnType, typename Class, typename... Args>
EntitiesWith<std::remove_reference_t<Args>...> EntitiesToProcessHelper( ReturnType( Class::*Fn )( Args... )const ) {
  return EntitiesWith<std::remove_reference_t<Args>...>{};
}
}
template<typename T>
class System : public SystemBase {
public:
  System( World &world, const std::string & name );
  virtual bool HasEntities( ) const final { return SystemTraits<T>::HasEntities; }
  virtual bool HasVoidUpdate( ) const final { return SystemTraits<T>::HasVoidUpdate; }
  virtual bool HasDtUpdate( ) const final { return SystemTraits<T>::HasDtUpdate; }
  virtual bool HasFixedUpdate( ) const final { return SystemTraits<T>::HasFixedUpdate; }
  virtual bool HasEditorUpdate( ) const final { return SystemTraits<T>::HasEditorUpdate; }
  virtual bool HasFrameStart( ) const final { return SystemTraits<T>::HasFrameStart; }
  virtual bool HasFrameEnd( ) const final { return SystemTraits<T>::HasFrameEnd; }
  virtual bool HasPreProcess( ) const final { return SystemTraits<T>::HasPreProcess; }
  virtual bool HasProcess( ) const final { return SystemTraits<T>::HasProcess; }

  virtual bool IsPureSystem( ) const final { return SystemTraits<T>::IsPureSystem; }
  virtual bool IsParallelSystem( ) const { return SystemTraits<T>::IsParallelSystem; }

  virtual void OnUpdate( float Dt ) final {
    if constexpr( SystemTraits<T>::HasDtMember ) this->SetDt( Dt );
    if constexpr( SystemTraits<T>::HasFrameStart ) this->FrameStart( );
    if constexpr( SystemTraits<T>::HasPreProcess ) this->PreProcess( );
    if constexpr( SystemTraits<T>::HasProcess ) this->Process( );
    if constexpr( SystemTraits<T>::HasVoidUpdate ) this->VoidUpdate( );
    if constexpr( SystemTraits<T>::HasDtUpdate ) this->DtUpdate( Dt );
    if constexpr( SystemTraits<T>::HasEditorUpdate ) this->EditorUpdate( Dt );
    if constexpr( SystemTraits<T>::HasFixedUpdate ) this->FixedUpdate( Dt );

    if constexpr( SystemTraits<T>::HasFrameEnd ) this->FrameEnd( );
  }

private:
  template<typename U = T>
  void FrameStart( typename std::enable_if_t<SystemTraits<U>::HasFrameStart>* = nullptr ) { instance.FrameStart( ); }
  template<typename U = T >
  void FrameStart( typename std::enable_if_t<!SystemTraits<U>::HasFrameStart>* = nullptr ) { }

  template<typename U = T>
  void PreProcess( typename std::enable_if_t<SystemTraits<U>::HasPreProcess>* = nullptr ) { instance.PreProcess( ); }
  template<typename U = T >
  void PreProcess( typename std::enable_if_t<!SystemTraits<U>::HasPreProcess>* = nullptr ) { }

  template<typename U = T>
  void Process( typename std::enable_if_t<SystemTraits<U>::HasProcess>* = nullptr ) {
    for( auto entity : s.Entities ) {
      entity.Invoke( [ & ]( auto&&... args ) {
        instance.Process( std::forward<decltype( args )>( args )... );
      } );
    }
  }
  template<typename U = T >
  void Process( typename std::enable_if_t<!SystemTraits<U>::HasProcess>* = nullptr ) { }

  template<typename U = T>
  void VoidUpdate( typename std::enable_if_t<SystemTraits<U>::HasVoidUpdate>* = nullptr ) { instance.Update( ); }
  template<typename U = T >
  void VoidUpdate( typename std::enable_if_t<!SystemTraits<U>::HasVoidUpdate>* = nullptr ) { }

  template<typename U = T>
  void DtUpdate( float Dt, typename std::enable_if_t<SystemTraits<U>::HasDtUpdate>* = nullptr ) { instance.Update( Dt ); }
  template<typename U = T >
  void DtUpdate( float Dt, typename std::enable_if_t<!SystemTraits<U>::HasDtUpdate>* = nullptr ) { }

  template<typename U = T>
  void FixedUpdate( float Dt, typename std::enable_if_t<SystemTraits<U>::HasFixedUpdate>* = nullptr ) { 
    constexpr float FixedTimestep{ 1.f / 60.f };
    s.Time += Dt;
    if( Dt >= FixedTimestep ) Dt -= FixedTimestep;
    instance.FixedUpdate( ); 
  }
  template<typename U = T >
  void FixedUpdate( float Dt, typename std::enable_if_t<!SystemTraits<U>::HasFixedUpdate>* = nullptr ) { }

  template<typename U = T>
  void FrameEnd( typename std::enable_if_t<SystemTraits<U>::HasFrameEnd>* = nullptr ) { instance.FrameEnd( ); }
  template<typename U = T >
  void FrameEnd( typename std::enable_if_t<!SystemTraits<U>::HasFrameEnd>* = nullptr ) { }

  template<typename U = T>
  void SetDt( float Dt, typename std::enable_if_t<SystemTraits<U>::HasDtMember>* = nullptr ) { instance.Dt = Dt; }
  template<typename U = T >
  void SetDt( float Dt, typename std::enable_if_t<!SystemTraits<U>::HasDtMember>* = nullptr ) { }

  virtual void AddSystem( World &world ) final;
  void RegisterEntities( World &world );
  void RegisterEditorUpdate( World & world );
  void RegisterParallelSystemProcess( World &world );
  T instance;
  std::string m_name;


  // Special storage types using SFINAE to determine what to store
  // Additional data needs to be stored depending on traits of the System type. This specializes to have the right data. 
  template<typename U = T, bool Process = SystemTraits<T>::HasProcess, bool Timed = SystemTraits<T>::HasFixedUpdate>
  struct Storage { };
  template<>
  struct Storage<T, true, false> {
    decltype( detail::EntitiesToProcess( std::declval<const T&>() ) ) Entities;
  };
  template<>
  struct Storage<T, true, true> {
    decltype( detail::EntitiesToProcess( std::declval<const T&>( ) ) ) Entities;
    float Time{ 0.f };
  };
  Storage<T> s;
};

#include "World.hpp"
template<typename T>
inline System<T>::System( World &world, const std::string & name )
  : m_name( name ) {
  AddSystem( world );
  world.On<UpdateEvent>( [ & ]( const UpdateEvent& ue ) { OnUpdate( ue.Dt ); } );
}
template<typename T>
inline void System<T>::AddSystem( World & world ) { 
  if constexpr( SystemTraits<T>::HasEntities ) {
    this->RegisterEntities( world );
  }
  if constexpr( SystemTraits<T>::HasEditorUpdate ) {
    this->RegisterEditorUpdate( world );
  }
  if constexpr( SystemTraits<T>::IsParallelSystem ) {
    this->RegisterParallelSystemProcess( world );
  }
}

template<typename T>
inline void System<T>::RegisterEntities( World & world ) {
  world.RegisterEntitiesWith( instance.Entities );
}

template<typename T>
inline void System<T>::RegisterEditorUpdate( World & world ) { 
  world.On<EditorUpdateEvent>( [ & ]( const EditorUpdateEvent & ) { instance.EditorUpdate( ); }, m_name );
}

#include "EntitiesWith.hpp"

template<typename T>
inline void System<T>::RegisterParallelSystemProcess( World &world ) {
  world.RegisterEntitiesWith( s.Entities );
}

