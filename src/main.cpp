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
  ParallelGravity() = default;
  ParallelGravity(glm::vec3 gravity) : m_gravity(gravity) {}
  ParallelGravity(const ParallelGravity &) = default;
  ParallelGravity(ParallelGravity &&) = default;
  void PreProcess( ) { }
  void Process( RigidBody&rb ) const {
    rb.acceleration += m_gravity * Dt;
  }
  glm::vec3 m_gravity{ 0.f,-9.81f,0.f };
  float Dt{ 0.f };
};

// ---------------------------
// *  Global Settings INIT  * 
// ---------------------------
float g_InitialWindowWidth = 100.0;
float g_InitialWindowHeight = 100.0;
bool g_StartFullscreen = false;
// ---------------------------

#include "Simulation.hpp"
#include "World.hpp"
#include "RenderSystem.h"
#include "SettingsFileReader.hpp"


void ECSDemo( ) {
  Simulation Sim;
  World &TestWorld{ Sim.CreateWorld( "Test World" ) };

  //Load Global Game Settings - Always do this before adding systems!
  SettingsFile settings{ "Settings.ini" };
  settings.Load();

  TestWorld.AddSystem<WindowManager>( "Window Management System" );
  TestWorld.AddSystem<Gravity>("Gravity System");
  TestWorld.AddSystem<ParallelGravity>("Parallelized Gravity System", glm::vec3{ 0.f, -9.81f, 0.f });
  TestWorld.AddSystem<ParallelVelocitySystem>( "Parallelized Velocity System" );
  TestWorld.AddSystem<ParallelAccelerationSystem>( "Parallelized Acceleration System" );
  TestWorld.AddSystem<RenderSystem>("Rendering System");



  ArchetypeRef enemy{ Sim.CreateArchetype( "Nanosuit Character" ) };
  ArchetypeRef lens{ Sim.CreateArchetype( "Camera Lens" ) };
  lens.Add<Camera>();
  enemy.Add<Transform>( ).scale = { 0.2f, 0.2f, 0.2f };
  enemy.Get<Transform>( ).pos = { 0.0f, 0.0f, -3.0f };
  enemy.Get<Transform>().rot = { 0.f, 0.f, 0.f };
  enemy.Add<RigidBody>( );
  CModel& cm{ enemy.Add<CModel>("nanosuit.obj") };
  cm.model->Load();
  EntityRef cam{ TestWorld.Spawn(lens) };
  cam.Get<Camera>().position = glm::vec3{ 4.5f, 3.4f, 14.26f };
  cam.Get<Camera>().pitch = -11.f;
  cam.Get<Camera>().yaw = -89.f;
  EntityRef EnemyA{ TestWorld.Spawn( enemy ) };
  // set SpawnNanos to false if you want higher FPS and less nanosuits
  if (bool SpawnNanos{ true }; SpawnNanos) {
    std::vector<EntityRef> nanos;
    for (int i{ 0 }; i < 2; ++i) {
      for (int j{ 0 }; j < 2; ++j) {
        nanos.emplace_back(EnemyA.Clone());
        nanos.back().Get<Transform>().pos = glm::vec3{ i, 0, j };
      }
    }
  }
  bool WindowOpen = true;
  TestWorld.On<GLWindow::EWindowStateChanged>([&](const GLWindow::EWindowStateChanged &event) {
    if(event.newState == WindowState::closed) WindowOpen = false;
  });
  while( WindowOpen ) {
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
