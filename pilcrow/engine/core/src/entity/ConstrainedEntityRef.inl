#pragma once

template<typename... Args>
template<typename Component>
typename std::enable_if_t<!std::is_const_v<Component>, Component&> ConstrainedEntityRef<Args...>::Get( ) {
  return this->GetMutable<Component, Args...>( ); 
}

template<typename... Args>
template<typename Component>
typename std::enable_if_t<std::is_const_v<Component>, const Component&> ConstrainedEntityRef<Args...>::Get( ) const {
  return this->GetImmutable<Component, Args...>( ); 
}

template<typename... Args>
template<typename Predicate>
void ConstrainedEntityRef<Args...>::Invoke( Predicate&& p ) { 
  p( Get<Args>( )... ); 
}
template<typename... Args>
template<typename ReturnType, typename Class>
void ConstrainedEntityRef<Args...>::Invoke( ReturnType( Class::*Fn )( Args... ) const, const Class& c ) { 
  ( c.*Fn )( Get<Args>( )... ); 
}

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
    } else if constexpr (std::is_same_v<Component, T2>) {
      return this->GetMutable<Component, T2>();
    } else if constexpr( sizeof...( FnArgs ) ) {
      return this->GetMutable<Component, FnArgs...>( );
    } else {
      static_assert(0, "Error: Trying to access a component on a constrained entity which does not have that component as a constraint.");
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


