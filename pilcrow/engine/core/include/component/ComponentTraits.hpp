#pragma once

#include "Detectors.hpp"

template<typename Component>
struct ComponentTraits {
  static_assert( IsComponent_v<Component>, "Error: type does not meet the requirements of a component. Components must be copy constructible." );
  static constexpr bool HasVoidUpdate = HasUpdateMemFn_v<Component>;
  static constexpr bool HasDtUpdate = HasUpdateDTMemFn_v<Component>;
  static constexpr bool HasFixedUpdate = HasFixedUpdateMemFn_v<Component>;
  static constexpr bool HasEditorUpdate = HasEditorUpdateMemFn_v<Component>;
  static constexpr bool HasFrameStart = HasFrameStartMemFn_v<Component>;
  static constexpr bool HasFrameEnd = HasFrameEndMemFn_v<Component>;
  static constexpr bool HasOwner = HasOwner_v<Component>;
  static constexpr bool IsPure = IsPureComponent_v<Component>;
};
