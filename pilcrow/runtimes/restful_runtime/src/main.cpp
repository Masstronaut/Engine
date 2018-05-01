#include <glad/include/glad.h> // MUST be included BEFORE glfw3.h
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <memory>

//TODO: Remove
#include "Camera.hpp"

Camera cam;
double dt{ 0.f };
double lastFrame{ 0.f };

void UnrecoverableError(const char* msg) {
	std::cout << "An unrecoverable error has occurred:\n";
	std::cout << msg << std::endl;
	system("pause");
	exit(-1);
}

std::string ReadFile(const std::string& path) {
	std::ifstream infile(path);
	std::string result;
	result.reserve(1024);
	std::array<char, 256> line;
	while (infile.good() && !infile.eof()) {
		infile.getline(line.data(), 256);
		result += line.data();
		result += "\n";
	}
	return result;
}

#include "Utils/include/ResourceSettings.h"
std::string RelativePath() {
	static std::string path{ g_ResourcePath };
	return path;
}
std::string ShaderPath() {
	static std::string path{ RelativePath() + "Shaders/" };
	return path;
}

#include "Detectors.hpp"

#include <Entity/EntitiesWith.hpp>

#include <Components/Transform.h>
struct RigidBody {
	glm::vec3 velocity{ 0.f,0.f,0.f }, acceleration{ 0.f,0.f,0.f };
	bool isStatic{ false }, isGhost{ false };
};
struct CircleCollider {
	float radius{ 1.f };
};
template<typename T>
void ComponentPrinter(const T& comp) { std::cout << comp << std::endl; }

void TransformPrinter(const Transform& trans) {
	std::cout << "{\n  pos : { " << trans.position.x << ", " << trans.position.y << ", " << trans.position.z << " },\n  "
		<< "{\n  rot : { " << trans.rotation.x << ", " << trans.rotation.y << ", " << trans.rotation.z << " },\n  "
		<< "{\n  scale : { " << trans.scale.x << ", " << trans.scale.y << ", " << trans.scale.z << " }\n}\n";
}

class TransformPrinterSystem {
public:
	EntitiesWith<const Transform> Entities;
	void Update() {
		for (const auto& e : Entities)
			TransformPrinter(e.Get<const Transform>());
	}
};

class Gravity {
public:
	EntitiesWith<RigidBody> Entities;
	void Update(float dt) {
		for (auto & e : Entities) {
			auto &rb = e.Get<RigidBody>();
			rb.acceleration.y -= dt * 9.81f;
		}
	}
};
class VelocitySystem {
public:
	EntitiesWith<Transform, const RigidBody> Entities;
	void Update(float dt) {
		for (auto &e : Entities) {
			const auto &rb = e.Get<const RigidBody>();
			auto &tf = e.Get<Transform>();
			tf.position += rb.velocity * dt;
		}
	}
};
class AccelerationSystem {
public:
	EntitiesWith<RigidBody> Entities;
	void Update(float dt) {
		for (auto & e : Entities) {
			auto &rb = e.Get<RigidBody>();
			rb.velocity += rb.acceleration * dt;
		}
	}
};

struct ParallelVelocitySystem {
	void PreProcess() { }
	void Process(Transform &tf, const RigidBody &rb) const {
		tf.position += rb.velocity * Dt;
		if (tf.position.y <= 0.f) {
			tf.position.y = 0.f;
		}
	}
	float Dt = 1.f / 60.f;
};

struct ParallelAccelerationSystem {
	void PreProcess() { }
	void Process(RigidBody &rb) const {
		rb.velocity += rb.acceleration * Dt;
	}
	float Dt = 1.f / 60.f;
};

struct ParallelGravity {
	ParallelGravity() = default;
	ParallelGravity(glm::vec3 gravity) : m_gravity(gravity) {}
	ParallelGravity(const ParallelGravity &) = default;
	ParallelGravity(ParallelGravity &&) = default;
	void PreProcess() { }
	void Process(RigidBody&rb) const {
		rb.acceleration += m_gravity * Dt;
	}
	glm::vec3 m_gravity{ 0.f,-9.81f,0.f };
	float Dt{ 0.f };
};


// ---------------------------
// *  Global Settings INIT  * 
// ---------------------------
//window
float g_InitialWindowWidth = 100.0;
float g_InitialWindowHeight = 100.0;
bool g_StartFullscreen = false;
//resources
const char* g_ResourcePath = "../../../../Resources/";
//game
bool g_SpawnNanos = false;
// ---------------------------

#include <RESTAPI.h>
#include "Simulation.hpp"
#include "World.hpp"
#include "RenderSystem.h"
#include "SettingsFileReader.hpp"

#include <Systems/CollisionDetection.h>
#include <Systems/Integration.h>
#include <Systems/Resolution.h>

void ECSDemo() {
	Simulation Sim;
	World &TestWorld{ Sim.CreateWorld("Test World") };

	//Load Global Game Settings - Always do this before adding systems!
	SettingsFile settings{ "Settings.ini" };
	settings.Load();

	TestWorld.AddSystem<WindowManager>("Window Management System");
	//TestWorld.AddSystem<Gravity>("Gravity System");
	//TestWorld.AddSystem<ParallelGravity>("Parallelized Gravity System", glm::vec3{ 0.f, -9.81f, 0.f });
	//TestWorld.AddSystem<ParallelVelocitySystem>("Parallelized Velocity System");
	//TestWorld.AddSystem<ParallelAccelerationSystem>("Parallelized Acceleration System");
	TestWorld.AddSystem<Integration>("Physics Integration");
	TestWorld.AddSystem<CollisionDetection>("Physics Collision Detection");
	TestWorld.AddSystem<Resolution>("Physics Resolution");
	TestWorld.AddSystem<RenderSystem>("Rendering System");


	ArchetypeRef enemy{ Sim.CreateArchetype("Nanosuit Character") };
	ArchetypeRef lens{ Sim.CreateArchetype("Camera Lens") };
	lens.Add<Camera>();
	enemy.Add<RigidBody>();
	CModel& cm{ enemy.Add<CModel>( "nanosuit.obj") };
	
	float s = 0.f;
	s = cm.model->GetScale();
	enemy.Add<Transform>().scale = { s, s, s};
	enemy.Get<Transform>().position = { 0.0f, 0.0f, -3.0f };
	enemy.Get<Transform>().rotation = { 0.f, 0.f, 0.f };

	EntityRef cam{ TestWorld.Spawn(lens) };
	cam.Get<Camera>().position = glm::vec3{ 0.f, 0.6f, -2.f };
	cam.Get<Camera>().pitch = -1.f;
	cam.Get<Camera>().yaw = -89.f;
	EntityRef EnemyA{ TestWorld.Spawn(enemy) };
	
	// set SpawnNanos to false if you want higher FPS and less nanosuits
	std::vector<EntityRef> nanos;
	if (g_SpawnNanos)
	{
		for (int i{ 0 }; i < 10; ++i)
		{
			for (int j{ 0 }; j < 10; ++j)
			{
				nanos.emplace_back(EnemyA.Clone());
				nanos.back().Get<Transform>().position = glm::vec3{ i * 2, 0, j * 2 };
			}
		}

		//TODO: google test this
		//Model remove testing
		nanos[0].Remove<CModel>();

		//TODO: google test this
		//Model change testing
		CModel bunny{ "bunny.ply" };
		s = bunny.model->GetScale();
		nanos[1].Get<CModel>() = bunny;
		nanos[1].Get<Transform>().scale = { s,s,s };
	}
	
	

	//Makes the Game exit on window close
	bool WindowOpen = true;
	TestWorld.On([&](const Jellyfish::GLWindow::EWindowStateChanged &event)
	{
		if (event.newState == Jellyfish::WindowState::closed)
			WindowOpen = false;
	});


	try {

		REST_VM h(Sim, utility::string_t(U("http://*:42069/api/")));
		auto server = h.Open();
		while (WindowOpen) {
			double currentFrame = glfwGetTime();
			dt = currentFrame - lastFrame;
			unsigned FPS{ static_cast< unsigned >(1. / dt) };
			lastFrame = currentFrame;
			Sim.Run(dt, TestWorld.Name());
		}
		server.wait();
	}
	catch (...) {} // if run without admin perms the server throws on exit since it can't open a port.
}

#include <Mathematics.h>

int main() {

	ECSDemo();

	return 0;
}
