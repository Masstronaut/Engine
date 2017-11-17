#pragma once

#include <detection_idiom.hpp>



#define GENERATE_DETECT_HAS_VEC_FIELD( FIELD ) \
template<typename T> \
using FIELD##_field = decltype( std::declval<T&>( ).FIELD.x ); \
template<typename T> \
using has_##FIELD##_field = is_detected<FIELD##_field, T>; \
template<typename T> \
constexpr bool has_##FIELD##_field_v = has_##FIELD##_field<T>::value; \
template<typename T> \
constexpr size_t FIELD##_field_size = sizeof( std::conditional_t<has_##FIELD##_field_v<T>, decltype( std::declval<T&>( ).FIELD ), void_t<T>>);

GENERATE_DETECT_HAS_VEC_FIELD( position );
GENERATE_DETECT_HAS_VEC_FIELD( normal );
GENERATE_DETECT_HAS_VEC_FIELD( texCoord );
GENERATE_DETECT_HAS_VEC_FIELD( tangent );
GENERATE_DETECT_HAS_VEC_FIELD( bitangent );

#define GENERATE_DETECT_HAS_VOID_MEMFN( FN ) \
template<typename T> \
using FN##_memfn = decltype( std::declval<T&>( ).FN() ); \
template<typename T> \
using has_##FN##_memfn = is_detected<FN##_memfn, T>; \
template<typename T> \
constexpr bool has_##FN##_memfn_v = has_##FN##_memfn<T>::value; 

// MEMFN function detection
GENERATE_DETECT_HAS_VOID_MEMFN( Update );
GENERATE_DETECT_HAS_VOID_MEMFN( FixedUpdate ); 
GENERATE_DETECT_HAS_VOID_MEMFN( EditorUpdate );
GENERATE_DETECT_HAS_VOID_MEMFN( FrameStart );
GENERATE_DETECT_HAS_VOID_MEMFN( FrameEnd );
GENERATE_DETECT_HAS_VOID_MEMFN( Reset );
GENERATE_DETECT_HAS_VOID_MEMFN( Reload );
GENERATE_DETECT_HAS_VOID_MEMFN( Load );
GENERATE_DETECT_HAS_VOID_MEMFN( Unload );


// engine constraints detection
template<typename T>
constexpr bool is_pure_system_v{ std::is_empty_v<T> };

template<typename T>
constexpr bool is_pure_component_v{ std::is_pod_v<T> };


// Type list
// @@TODO: move this to a better home
template<typename T, typename... Args>
struct type_list {
  using front = T;
  using pop_front = type_list<Args...>;
};
template<typename T>
struct type_list<T> {
  using front = T;

};
template<typename T, typename... Args>
struct is_type_list : std::false_type { };

template<template<typename...> typename T, typename... Args>
struct is_type_list<T<Args...>> : public std::is_same<T<Args...>, type_list<Args...>> { };

template<typename T>
constexpr bool is_type_list_v = is_type_list<T>::value;

// special types detection
// Detect if a system has a list of components it wants to iterate over
template<typename T>
using RequiredComponents_exists = decltype( is_type_list_v<typename T::RequiredComponents> );

template<typename T>
using has_RequiredComponents = is_detected<RequiredComponents_exists, T>;

template<typename T>
constexpr bool has_RequiredComponents_v = has_RequiredComponents<T>::value;
