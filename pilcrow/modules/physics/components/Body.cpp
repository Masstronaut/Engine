#include "pilcrow/modules/physics/components/Body.h"

void Body::SetMass(float m) {
  if(m <= Epsilon) {
    massInverse = Infinity;
  } else {
    massInverse = 1.0f / m;
  }
}

float Body::GetMass() const {
  if(massInverse == Infinity) { return 0.0f; }
  return 1.0f / massInverse;
}
