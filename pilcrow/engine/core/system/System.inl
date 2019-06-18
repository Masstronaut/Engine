#pragma once

#include "pilcrow/engine/core/system/System.hpp"

#include "pilcrow/engine/core/World.hpp"

template <typename T>
bool System<T>::HasEntities() const {
  return SystemTraits<T>::HasEntities;
}
template <typename T>
bool System<T>::HasVoidUpdate() const {
  return SystemTraits<T>::HasVoidUpdate;
}
template <typename T>
bool System<T>::HasDtUpdate() const {
  return SystemTraits<T>::HasDtUpdate;
}
template <typename T>
bool System<T>::HasFixedUpdate() const {
  return SystemTraits<T>::HasFixedUpdate;
}
template <typename T>
bool System<T>::HasEditorUpdate() const {
  return SystemTraits<T>::HasEditorUpdate;
}
template <typename T>
bool System<T>::HasFrameStart() const {
  return SystemTraits<T>::HasFrameStart;
}
template <typename T>
bool System<T>::HasFrameEnd() const {
  return SystemTraits<T>::HasFrameEnd;
}
template <typename T>
bool System<T>::HasPreProcess() const {
  return SystemTraits<T>::HasPreProcess;
}
template <typename T>
bool System<T>::HasProcess() const {
  return SystemTraits<T>::HasProcess;
}
template <typename T>
bool System<T>::IsPureSystem() const {
  return SystemTraits<T>::IsPureSystem;
}
template <typename T>
bool System<T>::IsParallelSystem() const {
  return SystemTraits<T>::IsParallelSystem;
}

template <typename T>
void System<T>::OnFrameStart() {
  if constexpr(SystemTraits<T>::HasFrameStart) this->FrameStart();
}
template <typename T>
void System<T>::OnUpdate(float Dt) {
  if constexpr(SystemTraits<T>::HasDtMember) this->SetDt(Dt);
  if constexpr(SystemTraits<T>::HasPreProcess) this->PreProcess();
  if constexpr(SystemTraits<T>::HasProcess) this->Process();
  if constexpr(SystemTraits<T>::HasPostProcess) this->PostProcess();
  if constexpr(SystemTraits<T>::HasVoidUpdate) this->VoidUpdate();
  if constexpr(SystemTraits<T>::HasDtUpdate) this->DtUpdate(Dt);
  if constexpr(SystemTraits<T>::HasEditorUpdate) this->EditorUpdate(Dt);
  if constexpr(SystemTraits<T>::HasFixedUpdate) this->FixedUpdate(Dt);
}
template <typename T>
void System<T>::OnFrameEnd() {
  if constexpr(SystemTraits<T>::HasFrameEnd) this->FrameEnd();
}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasFrameStart>
System<T>::FrameStart() {
  instance.FrameStart();
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasFrameStart>
System<T>::FrameStart() {}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasPreProcess>
System<T>::PreProcess() {
  instance.PreProcess();
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasPreProcess>
System<T>::PreProcess() {}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasProcess> System<T>::Process() {
  for(auto entity : s.Entities) {
    entity.Invoke([&](auto &&... args) {
      instance.Process(std::forward<decltype(args)>(args)...);
    });
  }
}

template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasProcess> System<T>::Process() {}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasPostProcess>
System<T>::PostProcess() {
  instance.PostProcess();
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasPostProcess>
System<T>::PostProcess() {}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasVoidUpdate>
System<T>::VoidUpdate() {
  instance.Update();
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasVoidUpdate>
System<T>::VoidUpdate() {}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasDtUpdate>
System<T>::DtUpdate(float Dt) {
  instance.Update(Dt);
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasDtUpdate>
System<T>::DtUpdate(float Dt) {}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasFixedUpdate>
System<T>::FixedUpdate(float Dt) {
  constexpr float FixedTimestep{1.f / 60.f};
  s.Time += Dt;
  if(Dt >= FixedTimestep) Dt -= FixedTimestep;
  instance.FixedUpdate();
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasFixedUpdate>
System<T>::FixedUpdate(float Dt) {}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasFrameEnd> System<T>::FrameEnd() {
  instance.FrameEnd();
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasFrameEnd> System<T>::FrameEnd() {
}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasDtMember>
System<T>::SetDt(float Dt) {
  instance.Dt = Dt;
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasDtMember>
System<T>::SetDt(float Dt) {}

template <typename T>
template <typename U>
typename std::enable_if_t<SystemTraits<U>::HasInitWorld>
System<T>::InitWorld(World &world) {
  instance.Init(world);
}
template <typename T>
template <typename U>
typename std::enable_if_t<!SystemTraits<U>::HasInitWorld>
System<T>::InitWorld(World &world) {}

template <typename T>
template <typename... Args>
inline System<T>::System(World &world, const std::string &name, Args &&... args)
  : instance(std::forward<Args>(args)...), m_name(name) {
  AddSystem(world);
  world.On([&](const UpdateEvent &ue) { OnUpdate(ue.Dt); });
  if constexpr(SystemTraits<T>::HasFrameStart)
    world.On([&](const FrameStartEvent &) { OnFrameStart(); });
  if constexpr(SystemTraits<T>::HasFrameEnd)
    world.On([&](const FrameEndEvent &) { OnFrameEnd(); });
  if constexpr(SystemTraits<T>::HasInitWorld) this->InitWorld(world);
}
template <typename T>
inline void System<T>::AddSystem(World &world) {
  if constexpr(SystemTraits<T>::HasEntities) { this->RegisterEntities(world); }
  if constexpr(SystemTraits<T>::HasEditorUpdate) {
    this->RegisterEditorUpdate(world);
  }
  if constexpr(SystemTraits<T>::IsParallelSystem) {
    this->RegisterParallelSystemProcess(world);
  }
}

template <typename T>
inline void System<T>::RegisterEntities(World &world) {
  world.RegisterEntitiesWith(instance.Entities);
}

template <typename T>
inline void System<T>::RegisterEditorUpdate(World &world) {
  world.On([&](const EditorUpdateEvent &) { instance.EditorUpdate(); }, m_name);
}

template <typename T>
inline void System<T>::RegisterParallelSystemProcess(World &world) {
  world.RegisterEntitiesWith(s.Entities);
}
