#pragma once
#include <detection_idiom.hpp>

class EntityRef;

namespace detail {
struct Wildcard {
  template <typename T>
  operator T();
};
}  // namespace detail

namespace detail {
// Implementation of is_pointer_to_const_member_function
// from https://stackoverflow.com/a/30407832/1514515
template <class T>
struct is_pointer_to_const_member_function : std::false_type {};

template <class R, class T, class... Args>
struct is_pointer_to_const_member_function<R (T::*)(Args...) const>
  : std::true_type {};

template <class R, class T, class... Args>
struct is_pointer_to_const_member_function<R (T::*)(Args...) const &>
  : std::true_type {};

template <class R, class T, class... Args>
struct is_pointer_to_const_member_function<R (T::*)(Args...) const &&>
  : std::true_type {};

template <class R, class T, class... Args>
struct is_pointer_to_const_member_function<R (T::*)(Args..., ...) const>
  : std::true_type {};

template <class R, class T, class... Args>
struct is_pointer_to_const_member_function<R (T::*)(Args..., ...) const &>
  : std::true_type {};

template <class R, class T, class... Args>
struct is_pointer_to_const_member_function<R (T::*)(Args..., ...) const &&>
  : std::true_type {};
};  // namespace detail

namespace detail {
// code from Ubsan (Nicole Mazzuca) on cpplang.slack.com
template <typename T>
struct is_member_function : std::false_type {};
// template <typename C, typename R, typename... Ts>
// struct is_member_function<R( C::* )( Ts... )> : std::true_type { };
template <typename C, typename R, typename... Ts>
struct is_member_function<R (C::*)(Ts...) const> : std::true_type {};

template <typename T>
constexpr bool is_member_function_v = is_member_function<T>::value;
}  // namespace detail

#define GENERATE_DETECT_HAS_VEC_FIELD(FIELD)                                   \
  template <typename T>                                                        \
  using FIELD##_field                                                          \
    = decltype(std::declval<T &>().FIELD.x + std::declval<T &>().FIELD.y);     \
                                                                               \
  template <typename T>                                                        \
  using has_##FIELD##_field = is_detected<FIELD##_field, T>;                   \
                                                                               \
  template <typename T>                                                        \
  constexpr bool has_##FIELD##_field_v = has_##FIELD##_field<T>::value;        \
                                                                               \
  template <typename T>                                                        \
  constexpr size_t FIELD##_field_size = sizeof(                                \
    std::conditional_t<has_##FIELD##_field_v<T>,                               \
                       decltype(std::declval<T &>().FIELD), void_t<T>>);

GENERATE_DETECT_HAS_VEC_FIELD(position);
GENERATE_DETECT_HAS_VEC_FIELD(normal);
GENERATE_DETECT_HAS_VEC_FIELD(texCoord);
GENERATE_DETECT_HAS_VEC_FIELD(tangent);
GENERATE_DETECT_HAS_VEC_FIELD(bitangent);

#define GENERATE_DETECT_HAS_VOID_MEMFN(FN)                                     \
                                                                               \
  template <typename T>                                                        \
  using FN##MemFn = decltype(std::declval<T &>().FN());                        \
                                                                               \
  template <typename T>                                                        \
  using Has##FN##MemFn = is_detected<FN##MemFn, T>;                            \
                                                                               \
  template <typename T>                                                        \
  constexpr bool Has##FN##MemFn_v = Has##FN##MemFn<T>::value;

// MEMFN function detection
GENERATE_DETECT_HAS_VOID_MEMFN(Update);
GENERATE_DETECT_HAS_VOID_MEMFN(FixedUpdate);
GENERATE_DETECT_HAS_VOID_MEMFN(EditorUpdate);
GENERATE_DETECT_HAS_VOID_MEMFN(FrameStart);
GENERATE_DETECT_HAS_VOID_MEMFN(FrameEnd);
GENERATE_DETECT_HAS_VOID_MEMFN(Reset);
GENERATE_DETECT_HAS_VOID_MEMFN(Reload);
GENERATE_DETECT_HAS_VOID_MEMFN(Load);
GENERATE_DETECT_HAS_VOID_MEMFN(Unload);
GENERATE_DETECT_HAS_VOID_MEMFN(PreProcess);
GENERATE_DETECT_HAS_VOID_MEMFN(PostProcess);

template <typename T>
using UpdateDTMemFn
  = decltype(std::declval<T &>().Update(std::declval<float>()));
template <typename T>
using HasUpdateDTMemFn = is_detected<UpdateDTMemFn, T>;
template <typename T>
constexpr bool HasUpdateDTMemFn_v = HasUpdateDTMemFn<T>::value;

class World;
template <typename T>
using InitWorldMemFn
  = decltype(std::declval<T &>().Init(std::declval<World &>()));
template <typename T>
using HasInitWorldMemFn = is_detected<InitWorldMemFn, T>;
template <typename T>
constexpr bool HasInitWorldMemFn_v = HasInitWorldMemFn<T>::value;

template <typename T>
using OperatorFnCall = decltype(T::operator());
template <typename T>
using HasOperatorFnCall = is_detected<OperatorFnCall, T>;
template <typename T>
constexpr bool HasOperatorFnCall_v = HasOperatorFnCall<T>::value;

template <typename T>
using OperatorFnCallFloat = decltype(std::declval<T &>(1.f));
template <typename T>
using HasOperatorFnCallFloat = is_detected<OperatorFnCallFloat, T>;
template <typename T>
constexpr bool HasOperatorFnCallFloat_v = HasOperatorFnCallFloat<T>::value;

// engine constraints detection
template <typename T>
constexpr bool IsPureSystem_v = std::is_empty_v<T>;

template <typename... Args>
class EntitiesWith;

template <typename T, typename... Args>
struct IsEntitiesWith : std::false_type {};

template <template <typename...> typename T, typename... Args>
struct IsEntitiesWith<T<Args...>>
  : public std::is_same<T<Args...>, EntitiesWith<Args...>> {};

template <typename T>
constexpr bool IsEntitiesWith_v = IsEntitiesWith<T>::value;

// special types detection
// Detect if a system has a list of components it wants to iterate over
template <typename T>
using DetectEntitiesMember = decltype(T::Entities);
template <typename T>
using HasEntitiesMember = is_detected<DetectEntitiesMember, T>;

template <typename T, typename Enable = void>
struct HasEntities : std::false_type {};

template <typename T>
struct HasEntities<T, typename std::enable_if_t<HasEntitiesMember<T>::value>> {
  static constexpr bool value = IsEntitiesWith_v<decltype(T::Entities)>;
};

template <typename T>
constexpr bool HasEntities_v = HasEntities<T>::value;

// detect if a component wants to know its owner
template <typename T>
using DetectHasOwnerMember = decltype(T::Owner);
template <typename T>
using HasOwnerMember = is_detected<DetectHasOwnerMember, T>;
template <typename T, typename Enable = void>
struct HasOwner : std::false_type {};

template <typename T>
struct HasOwner<T, typename std::enable_if_t<HasOwnerMember<T>::value>> {
  static constexpr bool value = std::is_same_v<decltype(T::Owner), EntityRef>;
};
template <typename T>
constexpr bool HasOwner_v = HasOwner<T>::value;

template <typename T, typename = void>
struct HasProcessMemFn : std::false_type {};

template <typename T>
struct HasProcessMemFn<
  T, std::enable_if_t<detail::is_member_function_v<decltype(&T::Process)>>>
  : std::true_type {};

template <typename T>
constexpr bool HasProcessMemFn_v = HasProcessMemFn<T>::value;

// clang-format off
template <typename T>
constexpr bool IsParallelSystem_v = HasPreProcessMemFn_v<T> 
                                 && HasProcessMemFn_v<T>;
//clang-format on

template <typename T>
using DetectDtMember = decltype(std::declval<T &>().Dt = 1.f);
template <typename T>
using HasDtMember = is_detected<DetectDtMember, T>;
template <typename T>
constexpr bool HasDtMember_v = HasDtMember<T>::value;

// composition of all minimum requirements for a component
template <typename T>
constexpr bool IsComponent_v = std::is_copy_constructible_v<T>;

// Clang-format gets it really wrong on chained boolean operators
// clang-format off
template <typename T>
constexpr bool IsPureComponent_v
  = std::is_pod_v<T> 
  && !HasUpdateMemFn_v<T> 
  && !HasUpdateDTMemFn_v<T> 
  && !HasEditorUpdateMemFn_v<T> 
  && !HasOwner_v<T> 
  && !HasFrameStartMemFn_v<T> 
  && !HasFrameEndMemFn_v<T>;
// clang-format on
