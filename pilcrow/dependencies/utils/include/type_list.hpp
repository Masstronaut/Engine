#pragma once

// Type list
template<typename T, typename... Args>
struct type_list {
  using front = T;
  using pop_front = type_list<Args...>;
  template<typename T2>
  using push_front = type_list<T2, T, Args...>;
  template<typename T2>
  using push_back = type_list<T, Args..., T2>;
};
template<typename T>
struct type_list<T> {
  using front = T;
  template<typename T2>
  using push_front = type_list<T2, T>;
  template<typename T2>
  using push_back = type_list<T, T2>;
};
template<typename T, typename... Args>
struct is_type_list : std::false_type { };

template<template<typename...> typename T, typename... Args>
struct is_type_list<T<Args...>> : public std::is_same<T<Args...>, type_list<Args...>> { };

template<typename T>
constexpr bool is_type_list_v = is_type_list<T>::value;
