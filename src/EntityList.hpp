#pragma once
#include <type_traits>

#include "EntityRef.hpp"
template<typename... Args>
class EntityList {
public:
  class ConstrainedEntityRef {
  public:
    template<typename Component, typename = std::enable_if_t<!std::is_const_v<Component>>>
    Component& Get( ) {
      return GetMutable<Component, Args...>( );
    }

    template<typename Component, typename = std::enable_if_t<std::is_const_v<Component>>>
    const T& Get( ) const {
      return GetImmutable<Component, Args...>( );
    }
  private:
    template<typename Component, typename T1>
    Component& GetMutable( ) {
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
      }
      static_assert( 0, "An error has occurred in accessing a component from an entity with it." );
    }
    template<typename Component, typename T1, typename T2, typename... Args>
    Component& GetMutable( ) {
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
      if constexpr( !std::is_same_v<Component, T1> ) {
        return GetMutable<T2, Args...>( );
      }
    }


    template<typename Component, typename T1>
    const Component& GetImmutable( ) {
      static_assert( std::is_const_v<Component>, "This shouldn't happen. Please send a bug report to Allan." );
      static_assert( !std::is_reference_v<Component>,
                     "References cannot be components. Try removing the reference from your component list." );
      static_assert( std::is_same_v<Component, std::add_const_t<T1>>,
                     "Accessing components not listed as required is not allowed." );
      static_assert( std::is_const_v<T1>,
                     "Error: Cannot get non-const access to a component required as const." );
      if constexpr ( std::is_same_v<Component, T1> ) {
        // at this point the component is the right type, and can be given to the user:
        return m_Self.Get<Component>( );
      }
      static_assert( 0, "An error has occurred in accessing a component from an entity with it." );
    }
    template<typename Component, typename T1, typename T2, typename... Args>
    const Component& GetImmutable( ) {
      static_assert( std::is_const_v<Component>, "This shouldn't happen. Please send a bug report to Allan." );
      static_assert( !std::is_reference_v<Component>,
                     "References cannot be components. Try removing the reference from your component list." );
      if constexpr( std::is_same_v<Component, std::add_const_t<T1>> ) {
        static_assert( std::is_const_v<T1>,
                       "Error: Trying to get a non-const reference to a required component marked as const." );
        static_assert( std::is_same_v<Component, T1>,
                       "An unknown error has occurred. Please submit a bug report with your system source code." );
        return m_Self.Get<Component>( );
      }
      if constexpr( !std::is_same_v<Component, T1> ) {
        return GetMutable<T2, Args...>( );
      }
    }
    EntityRef m_Self;
  };


};
