#pragma once

#include "pilcrow/engine/core/Detectors.hpp"

template <typename System>
struct SystemTraits {
  static constexpr bool HasEntities      = HasEntities_v<System>;
  static constexpr bool HasVoidUpdate    = HasUpdateMemFn_v<System>;
  static constexpr bool HasDtUpdate      = HasUpdateDTMemFn_v<System>;
  static constexpr bool HasFixedUpdate   = HasFixedUpdateMemFn_v<System>;
  static constexpr bool HasEditorUpdate  = HasEditorUpdateMemFn_v<System>;
  static constexpr bool HasFrameStart    = HasFrameStartMemFn_v<System>;
  static constexpr bool HasFrameEnd      = HasFrameEndMemFn_v<System>;
  static constexpr bool HasInitWorld     = HasInitWorldMemFn_v<System>;
  static constexpr bool HasPreProcess    = HasPreProcessMemFn_v<System>;
  static constexpr bool HasPostProcess   = HasPostProcessMemFn_v<System>;
  static constexpr bool HasProcess       = HasProcessMemFn_v<System>;
  static constexpr bool HasDtMember      = HasDtMember_v<System>;
  static constexpr bool IsPureSystem     = IsPureSystem_v<System>;
  static constexpr bool IsParallelSystem = IsParallelSystem_v<System>;
};
