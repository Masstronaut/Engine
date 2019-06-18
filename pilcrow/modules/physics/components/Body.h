#pragma once

#include "../Mathematics.h"

struct Body {
  float restitution;

  float massInverse;
  Mat3  inertiaTensor;

  Vec3 velocity;
  Vec3 angularVelocity;

  Vec3 impulse;
  Vec3 angularImpulse;

  void  SetMass(float m);
  float GetMass() const;
};