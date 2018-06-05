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


  // C-Style function support
  template<typename T>
  void On(void(*Fn)(const T&));

  // Lambda support with deduction
  template<typename Pred>
  void On(Pred &&p);
  template<typename Pred, typename T>
  void On(void(Pred::*mf)(const T&), Pred p);
  template<typename Pred, typename T>
  void On(void(Pred::*mf)(const T&) const, Pred p);
  
  // When the listener should only be called for a specific event value
  // @@TODO: Add the same overloads for handling lambdas as the normal On()
  template<typename T>
  void On( const T &value, Callback<T> cb );
  template<typename T>
  void On( const T &value, std::function<void(void)> cb );

  // When the listener should only be called once
  // @@TODO: Add the same overloads as On()
  template<typename T>
  void OnNext(Callback<T> cb);

  // C-Style function support
  template<typename T>
  void OnNext(void(*Fn)(const T&));

  // Lambda support with deduction
  template<typename Pred>
  void OnNext(Pred &&p);
  template<typename Pred, typename T>
  void OnNext(void(Pred::*mf)(const T&), Pred p);
  template<typename Pred, typename T>
  void OnNext(void(Pred::*mf)(const T&) const, Pred p);

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
  this->Dispatcher<T>().On( cb );
}
template<typename T>
inline void EventArena::On(void(*Fn)(const T &)) {
  this->On(std::function<void(const T&)>(Fn));
}
template<typename Pred>
inline void EventArena::On(Pred && p) {
  this->On(&Pred::operator(), p);
}
template<typename Pred, typename T>
inline void EventArena::On(void(Pred::* mf)(const T &), Pred p) {
  this->Dispatcher<T>().On(std::function<void(const T&)>(p));
}
template<typename Pred, typename T>
inline void EventArena::On(void(Pred::* mf)(const T &) const, Pred p) {
  this->Dispatcher<T>().On(std::function<void(const T&)>(p));
}
template<typename T>
void EventArena::On( const T &value, Callback<T> cb ) {
  this->On( [ cb, V = value ]( const T &Value ) { if( Value == V ) cb( Value ); } );
}
template<typename T>
void EventArena::On( const T &value, std::function<void( void )> cb ) {
  this->On( [ cb, V = value ]( const T &Value ) { if( Value == V ) cb( ); } );
}

template<typename T>
inline void EventArena::OnNext(Callback<T> cb) {
  this->Dispatcher<T>().OnNext(cb);
}
template<typename T>
inline void EventArena::OnNext(void(*Fn)(const T &)) {
  this->OnNext(std::function<void(const T&)>(Fn));
}

template<typename Pred>
inline void EventArena::OnNext(Pred && p) {
  this->OnNext(&Pred::operator(), p);
}
template<typename Pred, typename T>
inline void EventArena::OnNext(void(Pred::* mf)(const T &), Pred p) {
  this->Dispatcher<T>().OnNext(std::function<void(const T&)>(p));
}
template<typename Pred, typename T>
inline void EventArena::OnNext(void(Pred::* mf)(const T &) const, Pred p) {
  this->Dispatcher<T>().OnNext(std::function<void(const T&)>(p));
}
template<typename T>
inline void EventArena::Emit( const T &event ) {
  this->Dispatcher<T>( ).Emit( event );
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
