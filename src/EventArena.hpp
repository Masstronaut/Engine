#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <typeinfo>
class EventArena {
public:
  EventArena( ) = default;
  EventArena( const EventArena& ) = delete;
  EventArena( EventArena&& ) = default;
  template<typename T>
  using Callback = std::function<void( const T& )>;
  template<typename T>
  void On( Callback<T> cb );
  template<typename T>
  void On( const T &value, Callback<T> cb );
  template<typename T>
  void On( const T &value, std::function<void(void)> cb );
  template<typename T>
  void OnNext(Callback<T> cb);

  template<typename T>
  void Emit( const T &event ); // may provide option to emit instantly or delayed at a later time


private:
  using KeyType = std::type_index;
  struct EventDispatcherBase{ };
  template<typename T>
  class EventDispatcher : public EventDispatcherBase {
  public:
    using Callback = typename EventArena::Callback<T>;
    void Emit( const T &event );
    void On( Callback cb );
    void OnNext(Callback cb);
  private:
    std::vector<Callback> m_Callbacks;
    std::vector<Callback> m_CallOnce;
  };
  template<typename T>
  EventDispatcher<T>& Dispatcher( );
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
inline void EventArena::OnNext(Callback<T> cb) {
  Dispatcher<T>().OnNext(cb);
}

template<typename T>
void EventArena::Emit( const T &event ) {
  Dispatcher<T>( ).Emit( event );
}

template<typename T>
inline void EventArena::EventDispatcher<T>::Emit( const T & event ) {
  for( auto &cb : m_Callbacks ) cb( event );
  for (auto &cb : m_CallOnce) cb(event);
  m_CallOnce.clear();
}

template<typename T>
inline void EventArena::EventDispatcher<T>::On( Callback cb ) {
  m_Callbacks.emplace_back( cb );
}

template<typename T>
inline void EventArena::EventDispatcher<T>::OnNext(Callback cb) {
  m_CallOnce.emplace_back(cb);
}


template<typename T>
EventArena::EventDispatcher<T>& EventArena::Dispatcher( ) {
  auto it{ m_Dispatchers.find( std::type_index( typeid( T ) ) ) };
  if( it == m_Dispatchers.end( ) ) {
    auto res{ m_Dispatchers.try_emplace( std::type_index( typeid( T ) ), new EventDispatcher<T>{ } ) };
    if( res.second ) it = res.first;
  }
  return *( reinterpret_cast< EventDispatcher<T> * >( it->second.get( ) ) );
}