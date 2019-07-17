// ours
#include "pilcrow/modules/jellyfish_renderer/Vertex.h"

namespace Jellyfish {
Vertex::Vertex()
  : m_Position({0.f, 0.f, 0.f})
  , m_Normal({0.f, 0.f, 0.f})
  , m_Tangent({0.f, 0.f, 0.f})
  , m_Bitangent({0.f, 0.f, 0.f})
  , m_Color({1.f, 1.f, 1.f, 1.f})
  ,  // White
  m_TexCoords({0.f, 0.f, 0.f}) {}
}  // namespace Jellyfish
