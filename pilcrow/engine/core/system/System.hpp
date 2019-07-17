#pragma once
#include <functional>
#include <utility>  // forward

#include "pilcrow/engine/core/WorldEvents.hpp"
#include "pilcrow/engine/core/entity/EntitiesWith.hpp"

#include "pilcrow/engine/core/system/SystemTraits.hpp"

class World;
class SystemBase {
public:
  virtual ~SystemBase() {}
  virtual void AddSystem(World &world)  = 0;
  virtual bool HasEntities() const      = 0;
  virtual bool HasVoidUpdate() const    = 0;
  virtual bool HasDtUpdate() const      = 0;
  virtual bool HasFixedUpdate() const   = 0;
  virtual bool HasEditorUpdate() const  = 0;
  virtual bool HasFrameStart() const    = 0;
  virtual bool HasFrameEnd() const      = 0;
  virtual bool HasPreProcess() const    = 0;
  virtual bool HasProcess() const       = 0;
  virtual bool IsParallelSystem() const = 0;
  virtual bool IsPureSystem() const     = 0;

  virtual void OnFrameStart()     = 0;
  virtual void OnFrameEnd()       = 0;
  virtual void OnUpdate(float Dt) = 0;
};

namespace detail {

template <typename ReturnType, typename Class, typename... Args>
EntitiesWith<std::remove_reference_t<Args>...>
EntitiesToProcessHelper(ReturnType (Class::*Fn)(Args...) const) {
  return EntitiesWith<std::remove_reference_t<Args>...>{};
}
template <typename U>
auto EntitiesToProcess(
  const U &,
  typename std::enable_if_t<SystemTraits<U>::HasProcess> * = nullptr) {
  return EntitiesToProcessHelper(&U::Process);
}
template <typename U>
bool EntitiesToProcess(
  const U &,
  typename std::enable_if_t<!SystemTraits<U>::HasProcess> * = nullptr) {}

// Special storage types using SFINAE to determine what to store
// Additional data needs to be stored depending on traits of the System type.
// This specializes to have the right data.
template <typename T, bool Process = SystemTraits<T>::HasProcess,
          bool Timed = SystemTraits<T>::HasFixedUpdate>
struct Storage {};
template <typename T>
struct Storage<T, true, false> {
  decltype(detail::EntitiesToProcess(std::declval<const T &>())) Entities;
};
template <typename T>
struct Storage<T, true, true> {
  decltype(detail::EntitiesToProcess(std::declval<const T &>())) Entities;
  float                                                          Time{0.f};
};

}  // namespace detail

template <typename T>
class System : public SystemBase {
public:
  template <typename... Args>
  System(World &world, const std::string &name, Args &&... args);
  virtual bool HasEntities() const final;
  virtual bool HasVoidUpdate() const final;
  virtual bool HasDtUpdate() const final;
  virtual bool HasFixedUpdate() const final;
  virtual bool HasEditorUpdate() const final;
  virtual bool HasFrameStart() const final;
  virtual bool HasFrameEnd() const final;
  virtual bool HasPreProcess() const final;
  virtual bool HasProcess() const final;

  virtual bool IsPureSystem() const final;
  virtual bool IsParallelSystem() const;

  virtual void OnFrameStart() final;
  virtual void OnUpdate(float Dt) final;
  virtual void OnFrameEnd() final;

private:
  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasFrameStart> FrameStart();
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasFrameStart> FrameStart();

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasPreProcess> PreProcess();
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasPreProcess> PreProcess();

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasProcess> Process();
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasProcess> Process();

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasPostProcess> PostProcess();
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasPostProcess> PostProcess();

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasVoidUpdate> VoidUpdate();
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasVoidUpdate> VoidUpdate();

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasDtUpdate> DtUpdate(float Dt);
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasDtUpdate> DtUpdate(float Dt);

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasFixedUpdate>
  FixedUpdate(float Dt);
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasFixedUpdate>
  FixedUpdate(float Dt);

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasFrameEnd> FrameEnd();
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasFrameEnd> FrameEnd();

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasDtMember> SetDt(float Dt);
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasDtMember> SetDt(float Dt);

  template <typename U = T>
  typename std::enable_if_t<SystemTraits<U>::HasInitWorld>
  InitWorld(World &world);
  template <typename U = T>
  typename std::enable_if_t<!SystemTraits<U>::HasInitWorld>
  InitWorld(World &world);

  virtual void AddSystem(World &world) final;
  void         RegisterEntities(World &world);
  void         RegisterEditorUpdate(World &world);
  void         RegisterParallelSystemProcess(World &world);
  T            instance;
  std::string  m_name;

  detail::Storage<T> s;
};

#include "../system/System.inl"
