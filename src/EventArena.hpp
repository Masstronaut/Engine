#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
class EventArena {
public:
  template<typename T>
  using Callback = std::function<void( const T& )>;
  template<typename T>
  void On( Callback<T> cb );
  template<typename T>
  void On( const T &value, Callback<T> cb );
  template<typename T>
  void On( const T &value, std::function<void(void)> cb );

  template<typename T>
  void Emit( const T &event ); // may provide option to emit instantly or delayed at a later time


private:
  using KeyType = decltype( typeid( int ).hash_code() );
  struct EventDispatcherBase{ };
  template<typename T>
  class EventDispatcher : public EventDispatcherBase {
  public:
    using Callback = typename EventArena::Callback<T>;
    void Emit( const T &event ) {
      for( auto &cb : m_callbacks ) cb( event );
    }
    void On( Callback cb ) {
      m_callbacks.emplace_back( cb );
    }
  private:
    std::vector<Callback> m_callbacks;
  };
  template<typename T>
  EventDispatcher<T>& Dispatcher( ) { 
    auto it{ m_Dispatchers.find( typeid( T ).hash_code() ) };
    if( it == m_Dispatchers.end( ) ) {
      auto res{ m_Dispatchers.try_emplace( typeid( T ).hash_code(), new EventDispatcher<T>{} ) };
      if( res.second ) it = res.first;
    }
    return *(reinterpret_cast< EventDispatcher<T> * >( it->second.get( ) ));
  }
  std::unordered_map<KeyType, std::unique_ptr<EventDispatcherBase>> m_Dispatchers;
};
template<typename T>
void EventArena::On( Callback<T> cb ) {
  Dispatcher<T>().On( cb );
}
template<typename T>
void EventArena::On( const T &value, Callback<T> cb ) {
  On( [ cb, V = value ]( const T &Value ) { if( Value == V ) cb( value ); } );
}
template<typename T>
void EventArena::On( const T &value, std::function<void( void )> cb ) {
  On( [ cb, V = value ]( const T &Value ) { if( Value == V ) cb( ); } );
}

template<typename T>
void EventArena::Emit( const T &event ) {
  Dispatcher<T>( ).Emit( event );
}
