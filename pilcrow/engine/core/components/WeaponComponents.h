#pragma once
#include "pilcrow/engine/core/Entity/ArchetypeRef.hpp"
#include <cstdint>

enum class Keys { X, Y, A, B, L, R };
struct Weapon {
  float        TimeSinceFired{1.f};
  float        FireRate{1.f};
  uint16_t     CurrentAmmo{20};
  Keys         FireKey{Keys::R};
  ArchetypeRef BulletType;
  uint16_t     MaxAmmo{20};
};

struct ReloadableClip {
  Keys     ReloadKey{Keys::Y};
  float    ReloadSpeed{1.f};
  float    TimeSinceReloaded{1.f};
  uint16_t Clips{5};
  uint16_t MaxClips{10};
};

struct Bullet {
  float Damage{1.f};
};
struct Lifetime {
  float Lifetime{1.f};
};

struct ChargeWeapon {
  float MinChargeTime{1.f};
  float CurrentCharge{0.f};
  bool  LoseChargeOverTime{false};
  float MaxChargeTime{1.f};
};