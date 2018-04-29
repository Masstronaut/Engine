#pragma once
#include "../../include/component/ComponentTraits.hpp"
#include "../../include/component/ComponentPool.hpp"

template<typename Component>
inline ComponentPool<Component>::ComponentPool( World &world ) { 
  if constexpr( ComponentTraits<Component>::HasVoidUpdate ) {
    world.AddSystem( [ & ]( float dt ) {
      for( auto &comp : components ) comp.Update( );
    }, typeid( Component ).name( ) + "::Update()"s );
  }
  if constexpr( ComponentTraits<Component>::HasDtUpdate ) {
    world.AddSystem( [ & ]( float dt ) {
      for( auto &comp : components ) comp.Update( dt );
    }, typeid( Component ).name( ) + "::Update(dt)"s );
  }
  if constexpr( ComponentTraits<Component>::HasFixedUpdate ) {
    world.AddSystem( [ &, time = 0.f ]( float dt ) {
      time += dt;
      //@@TODO: change this to use whatever the fixed update frequency specified is
      if( time >= 1.f / 60.f ) {
        time -= 1.f / 60.f;
        for( auto &comp : components ) comp.Update( );
      }
    }, typeid( Component ).name( ) + "::Update()"s );
  }
}

template<typename Component>
inline void * ComponentPool<Component>::Get( EntityID ID ) {
  return &components[ ID ];
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

template<typename Component>
inline void ComponentPool<Component>::Erase(EntityID ID) {
  components.erase(ID);
  // @@OPTIMIZE: send out an event that the former .back() element has had its memory location changed.
}
