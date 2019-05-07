// Author: Allan Deutsch
// All content copyright (C) Allan Deutsch 2015. All rights reserved.

//Changelog:  2/25/2015 - Initial implementation. 

#pragma once
#include <utility>
namespace ADL
{

  // Use this macro to create a singleton class!
#define SINGLETON( classname ) \
class classname \
{ \
classname(); \
classname(const classname &) = delete; \
classname& operator=(const classname &) = delete; \
public: \
template <typename... Args>\
  static classname& get(Args... args) \
  { \
    static classname instance{ std::forward<Args>(args)... }; \
    return instance; \
  }

  template < typename T >
  class Singleton
  {
  public:
    template <typename... Args>
    static T& get(Args... args)
    {
      static T instance{ std::forward<Args>(args)... };
      return instance;
    }


  private:
    Singleton& operator=(const Singleton &) = delete;
    Singleton(const Singleton &) = delete;


  protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    

  };


}


