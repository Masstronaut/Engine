#pragma once

#include "Detectors.hpp"

template<typename System>
struct SystemTraits {
  static constexpr bool HasEntities = HasEntities_v<System>;
  static constexpr bool HasVoidUpdate = HasUpdateMemFn_v<System>;
  static constexpr bool HasDTUpdate = HasUpdateDTMemFn_v<System>;
  static constexpr bool HasFixedUpdate = HasFixedUpdateMemFn_v<System>;
  static constexpr bool HasEditorUpdate = HasEditorUpdateMemFn_v<System>;
  static constexpr bool HasFrameStart = HasFrameStartMemFn_v<System>;
  static constexpr bool HasFrameEnd = HasFrameEndMemFn_v<System>;
  
  static constexpr bool IsPureSystem = IsPureSystem_v<System>;
};
