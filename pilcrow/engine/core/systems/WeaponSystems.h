#pragma once
#include "pilcrow/engine/core/components/Transform.h"
#include "pilcrow/engine/core/components/WeaponComponents.h"

struct InputSource {
  bool KeyDown(Keys key) const { return key == Key; }
  Keys Key{Keys::R};
};
struct ParentRef {
  EntityRef Parent;
};
/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
struct WeaponSystem {
  EntityRef FireWeapon(World &world, const Weapon &weapon, const Transform &tf,
                       const ParentRef *parent = nullptr) const {
    EntityRef bullet{world.Spawn(weapon.BulletType)};
    if(bullet.Has<Transform>()) {
      Transform &btf{bullet.Get<Transform>()};
      btf.position = tf.position;
      btf.rotation = tf.rotation;
    }
    if(parent && bullet.Has<ParentRef>())
      bullet.Get<ParentRef>().Parent = parent->Parent;
    return bullet;
  }
  void Process(Weapon &weapon, const Transform &tf, const InputSource &input,
               const ParentRef &parent) const {
    weapon.TimeSinceFired += Dt;
    if(weapon.TimeSinceFired < weapon.FireRate) return;
    if(!weapon.CurrentAmmo) return;
    if(!input.KeyDown(weapon.FireKey)) return;
    weapon.TimeSinceFired = 0.f;
    weapon.CurrentAmmo -= 1.f;
    this->FireWeapon(*world, weapon, tf, &parent);
  }
  World *world{nullptr};
  float  Dt{1.f / 60.f};
};

struct ReloadSystem {
  void Process(Weapon &weapon, ReloadableClip &clip,
               const InputSource &input) const {
    clip.TimeSinceReloaded += Dt;
    if(clip.TimeSinceReloaded < clip.ReloadSpeed) {
      // Don't let the weapon fire while reloading.
      weapon.TimeSinceFired = 0.f;
      return;
    }
    if(!clip.Clips) return;
    if(!input.KeyDown(clip.ReloadKey)) return;
    weapon.CurrentAmmo = weapon.MaxAmmo;
    --clip.Clips;
  }
  float Dt{1.f / 60.f};
};

struct ChargeWeaponSystem {
  void Process(Weapon &weapon, const InputSource &input, ChargeWeapon &cw,
               const Transform &tf) const {
    weapon.TimeSinceFired = 0.f;
    if(!input.KeyDown(weapon.FireKey)) {
      this->TryFireWeapon(weapon, cw, cw.MinChargeTime);
      if(!cw.LoseChargeOverTime)
        cw.CurrentCharge = 0.f;
      else
        cw.CurrentCharge -= std::min(Dt, cw.CurrentCharge);  // Assuming
                                                             // Currentcharge
                                                             // can't go below 0
    } else {
      cw.CurrentCharge += Dt;
      if(cw.CurrentCharge < cw.MinChargeTime) return;
      this->TryFireWeapon(weapon, cw, cw.MaxChargeTime);
    }
  }
  void TryFireWeapon(Weapon &weapon, ChargeWeapon &cw, float TimeNeeded) const {
    if(cw.CurrentCharge >= TimeNeeded) {
      weapon.TimeSinceFired = weapon.FireRate;
      cw.CurrentCharge      = 0.f;
    }
  }
  float Dt{0.f};
};
