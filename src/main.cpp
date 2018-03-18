#include <glad/glad.h> // MUST be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <memory>

#include "Camera.hpp"
Camera cam;
double dt{ 0.f };
double lastFrame{ 0.f };

void UnrecoverableError( const char* msg ) {
  std::cout << "An unrecoverable error has occurred:\n";
  std::cout << msg << std::endl;
  system( "pause" );
  exit( -1 );
}

std::string ReadFile( const std::string& path ) {
  std::ifstream infile( path );
  std::string result;
  result.reserve( 1024 );
  std::array<char, 256> line;
  while( infile.good( ) && !infile.eof( ) ) {
    infile.getline( line.data( ), 256 );
    result += line.data( );
    result += "\n";
  }
  return result;
}

std::string RelativePath( ) {
  static std::string path{ "../" };
  return path;
}
std::string ShaderPath( ) {
  static std::string path{ RelativePath( ) + "Resources/Shaders/" };
  return path;
}

#include "Detectors.hpp"

#include "Entity/EntitiesWith.hpp"

#include "Components/Transform.h"
struct RigidBody {
  glm::vec3 velocity{ 0.f,0.f,0.f }, acceleration{ 0.f,0.f,0.f };
  bool isStatic{ false }, isGhost{ false };
};
struct CircleCollider {
  float radius{ 1.f };
};
template<typename T>
void ComponentPrinter( const T& comp ) { std::cout << comp << std::endl; }

void TransformPrinter( const Transform& trans ) {
  std::cout << "{\n  pos : { " << trans.pos.x << ", " << trans.pos.y << ", " << trans.pos.z << " },\n  "
    << "{\n  rot : { " << trans.rot.x << ", " << trans.rot.y << ", " << trans.rot.z << " },\n  "
    << "{\n  scale : { " << trans.scale.x << ", " << trans.scale.y << ", " << trans.scale.z << " }\n}\n";
}

class TransformPrinterSystem {
public:
  EntitiesWith<const Transform> Entities;
  void Update( ) { 
    for( const auto& e : Entities ) 
      TransformPrinter( e.Get<const Transform>( ) );
  }
};

class Gravity {
public:
  EntitiesWith<RigidBody> Entities;
  void Update( float dt ) {
    for( auto & e : Entities ) {
      auto &rb = e.Get<RigidBody>( );
      rb.acceleration.y -= dt * 9.81f;
    }
  }
};
class VelocitySystem {
public:
  EntitiesWith<Transform, const RigidBody> Entities;
  void Update( float dt ) {
    for( auto &e : Entities ) {
      const auto &rb = e.Get<const RigidBody>( );
      auto &tf = e.Get<Transform>( );
      tf.pos += rb.velocity * dt;
    }
  }
};
class AccelerationSystem {
public:
  EntitiesWith<RigidBody> Entities;
  void Update( float dt ) {
    for( auto & e : Entities ) {
      auto &rb = e.Get<RigidBody>( );
      rb.velocity += rb.acceleration * dt;
    }
  }
};

struct ParallelVelocitySystem {
  void PreProcess( ) { }
  void Process( Transform &tf, const RigidBody &rb ) const {
    tf.pos += rb.velocity * Dt;
    if( tf.pos.y <= 0.f ) {
      tf.pos.y = 0.f;
    }
  }
  float Dt = 1.f / 60.f;
};

struct ParallelAccelerationSystem {
  void PreProcess( ) { }
  void Process( RigidBody &rb ) const {
    rb.velocity += rb.acceleration * Dt;
  }
  float Dt = 1.f / 60.f;
};

struct ParallelGravity {
  void PreProcess( ) { }
  void Process( RigidBody&rb ) const {
    rb.acceleration += m_gravity * Dt;
  }
  glm::vec3 m_gravity{ 0.f,-9.81f,0.f };
  float Dt{ 0.f };
};

#include "Simulation.hpp"
#include "World.hpp"
#include "RenderSystem.h"
void ECSDemo( ) {
  // tests that constraint checking is working correctly

  static_assert( SystemTraits<TransformPrinterSystem>::HasEntities );
  static_assert( SystemTraits<TransformPrinterSystem>::HasVoidUpdate );
  static_assert( SystemTraits<ParallelGravity>::HasPreProcess );
  static_assert( SystemTraits<ParallelGravity>::HasProcess );
  static_assert( SystemTraits<ParallelVelocitySystem>::IsParallelSystem );
  static_assert( HasProcessMemFn_v<ParallelVelocitySystem> );
  static_assert( HasPreProcessMemFn_v<ParallelVelocitySystem> );
  /*static_assert( SystemTraits<ParallelGravity>::IsParallelSystem );
  static_assert( SystemTraits<ParallelGravity>::HasPreProcess );
  static_assert( SystemTraits<ParallelGravity>::HasProcess );*/
  Simulation Sim;
  World &TestWorld{ Sim.CreateWorld( "Test World" ) };
  //TestWorld.AddSystem<Gravity>( "Gravity" );
  //TestWorld.AddSystem<AccelerationSystem>( "Acceleration" );
  //TestWorld.AddSystem<VelocitySystem>( "Velocity" );
  TestWorld.AddSystem<WindowManager>( "Window Management System" );
  TestWorld.AddSystem<ParallelGravity>( "Parallelized Gravity System" );
  TestWorld.AddSystem<ParallelVelocitySystem>( "Parallelized Velocity System" );
  TestWorld.AddSystem<ParallelAccelerationSystem>( "Parallelized Acceleration System" );
  TestWorld.AddSystem<TransformPrinterSystem>( "Transform Printer" );
  TestWorld.AddSystem<RenderSystem>("Rendering System");

  ArchetypeRef enemy{ Sim.CreateArchetype( "Nanosuit Character" ) };
  ArchetypeRef lens{ Sim.CreateArchetype( "Camera Lens" ) };
  lens.Add<Camera>( );
  enemy.Add<Transform>( ).scale = { 0.2f, 0.2f, 0.2f };
  enemy.Get<Transform>( ).pos = { -1.0f, -3.0f, 3.0f };
  enemy.Add<RigidBody>( );
  CModel& cm{ enemy.Add<CModel>("nanosuit.obj") };
  cm.model->Load();
  TestWorld.Spawn( lens );
  EntityRef EnemyA{ TestWorld.Spawn( enemy ) };
  EntityRef EnemyB{ EnemyA.Clone( ) };
  EnemyB.Get<Transform>( ).pos.x = 1.f;
  EnemyB.Get<Transform>().pos.z = 3.f;
  while( true ) {
    double currentFrame = glfwGetTime();
    dt = currentFrame - lastFrame;
    unsigned FPS{ static_cast< unsigned >(1. / dt) };
    lastFrame = currentFrame;
    Sim.Run( dt, TestWorld.Name() );
  }
}

int main( ) {
  ECSDemo( );
  return 0;
}
