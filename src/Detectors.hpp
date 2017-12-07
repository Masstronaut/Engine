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

#include "type_list.hpp"
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
