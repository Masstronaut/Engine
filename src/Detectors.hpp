#pragma once
#include <detection_idiom.hpp>

namespace detail {
struct Wildcard {
  template<typename T>
  operator T( );
};
}

#define GENERATE_DETECT_HAS_VEC_FIELD( FIELD ) \
template<typename T> \
using FIELD##_field = decltype( std::declval<T&>( ).FIELD.x + std::declval<T&>( ).FIELD.y ); \
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
using FN##MemFn = decltype( std::declval<T&>( ).FN() ); \
template<typename T> \
using Has##FN##MemFn = is_detected<FN##MemFn, T>; \
template<typename T> \
constexpr bool Has##FN##MemFn_v = Has##FN##MemFn<T>::value; 

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

template<typename T>
using UpdateDTMemFn = decltype( std::declval<T&>( ).Update( std::declval<float>() ) );
template<typename T>
using HasUpdateDTMemFn = is_detected<UpdateDTMemFn, T>;
template<typename T>
constexpr bool HasUpdateDTMemFn_v = HasUpdateDTMemFn<T>::value;


// engine constraints detection
template<typename T>
constexpr bool IsPureSystem_v = std::is_empty_v<T>;

template<typename T>
constexpr bool IsPureComponent_v = std::is_pod_v<T>;
template<typename... Args>
class EntitiesWith;

template<typename T, typename... Args>
struct IsEntitiesWith : std::false_type { };

template<template<typename...> typename T, typename... Args>
struct IsEntitiesWith<T<Args...>> : public std::is_same<T<Args...>, EntitiesWith<Args...>> { };

template<typename T>
constexpr bool IsEntitiesWith_v = IsEntitiesWith<T>::value;

// special types detection
// Detect if a system has a list of components it wants to iterate over
template<typename T>
using DetectEntitiesMember = decltype(T::Entities);

template<typename T>
using HasEntities = is_detected<DetectEntitiesMember, T>;

template<typename T>
constexpr bool HasEntities_v = HasEntities<T>::value && IsEntitiesWith_v<decltype(T::Entities)>;


// composition of all minimum requirements for a component
template<typename T>
constexpr bool is_component = std::is_copy_constructible_v<T>;
