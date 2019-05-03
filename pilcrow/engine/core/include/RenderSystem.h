#pragma once
#include "Entity/EntitiesWith.hpp"
#include "RenderComponents.h"
#include "Components/Transform.h"

#include "SettingsFileReader.hpp"
#include <Jellyfish.h>


struct WindowManager : public EventArena
{
  WindowManager();

  void Init(World& world);
  void FrameStart( );
  void FrameEnd( );
  
  float Dt{ 0.f };

private:
  //render context
  Jellyfish::iWindow* pWindow{nullptr};
  
  void ProcessInput();

  glm::vec2 m_windowSizeSetting{ g_InitialWindowWidth, g_InitialWindowHeight };
  bool m_windowFullscreenSetting{ g_StartFullscreen };
};


class CameraManager : public EventArena
{
public:

#pragma region EventTypes

#pragma endregion

	//TODO: make it as system and pass the world in here
	void Init()
	{

	}

	void RegisterCamera(std::shared_ptr<Jellyfish::iCamera> camera, std::string id = "")
	{
		//if a string id was not supplied, we'll use a number
		if (id == "")
		{
			id = std::to_string(this->idCounter++);
		}

		m_cameras[id] = camera;

		//if no camera has been set as current, set this one
		if (currentCamera == nullptr)
			currentCamera = camera;

		return;
	}

	void SetCameraByID(std::string id)
	{
		currentCamera = m_cameras[id];
	}

	//TODO: const correctness
	std::shared_ptr<Jellyfish::iCamera> GetCurrentCamera() const
	{
		return currentCamera;
	}

	void Update(float dt)
	{
		currentCamera->Update(dt);
	}

	glm::mat4 View() const
	{
		return currentCamera->GetView();
	}

	glm::mat4 Projection() const
	{
		return currentCamera->GetProjection();
	}

private:
	std::unordered_map<std::string, std::shared_ptr<Jellyfish::iCamera>> m_cameras;
	std::shared_ptr<Jellyfish::iCamera> currentCamera = nullptr;
	int idCounter = 0;
};

struct RenderSystem 
{
	
	void Init(World& world)
	{
		//Register for Type Lists
		world.RegisterEntitiesWith(textEntities);

		//Event Listeners
		world.On([&](const Jellyfish::GLWindow::EWindowResized &event) 
		{
			m_windowSize = event.newSize;
		});
		
		//add a camera to the manager every time one is created
		world.On([&](const Jellyfish::iCamera::ECameraCreated &event)
		{
			m_CameraManager.RegisterCamera(event.pCamera);
		});
		
		//Load shader -- probably should move this
		program.Load();
	}

	void PreProcess() 
	{	
		//set perspective on current camera
		std::shared_ptr<Jellyfish::iCamera> cam = m_CameraManager.GetCurrentCamera();

		//no camera to render with in this case
		if (!cam)
			return;

		cam->SetPerspectiveProjection(m_windowSize.x, m_windowSize.y);
		//set up projetion matrices
		program.SetUniform("projection", m_CameraManager.Projection());
		program.SetUniform("view", m_CameraManager.View());
	}

	void Process(const CModel &model, const Transform &tf) const
	{	
		//TODO: implement bucket-based rendering.
		//Functionality will mustly be implemented in jellyfish, but the API calls related to entities will happen here.

		//for each (entity e)
		//{
		//	submit_draw_call_to_bucket(shadow_map, e);
		//	submit_draw_call_to_bucket(g_buffer, e);
		//	...
		//}

		//TODO: Buckets will be filled in Jellyfish backend based on the states assigned to the object being drawn
		//template <typename T>
		//class CommandBucket
		//{
		//	typedef T Key;
		//	...
		//
		//private:
		//	Key * m_keys;
		//	void** m_data;
		//};
		//example usage:
		//CommandBucket<uint64_t> gBufferBucket(2048, rt1, rt2, rt3, rt4, dst, someCamera->viewMatrix, someCamera->projMatrix);

		
		//you HAVE to ask the renderQueue to hand a draw call to you :
		//EXAMPLE:
		//IndexedDrawCall* dc = renderQueue::CreateIndexedDrawCall();
		//dc->SetVertexBuffer(mesh->vertexBuffer);
		//dc->SetIndexBuffer(mesh->indexBuffer);
		//dc->SetCullState(CULLSTATE_BACK);
		//renderQueue::Submit(dc);

		//using commands, --not yet implemented
		//for (size_t i = 0; i < meshComponents.size(); ++i)
		//{
		//	MeshComponent* mesh = &meshComponents[i];
		//
		//	commands::DrawIndexed* dc = gBuffer.AddCommand<commands::DrawIndexed>(GenerateKey(mesh->aabb, mesh->material));
		//	dc->vertexLayoutHandle = mesh->vertexLayout;
		//	dc->vertexBuffer = mesh->vertexBuffer;
		//	dc->indexBuffer = mesh->indexBuffer;
		//	dc->indexCount = mesh->indexCount;
		//	dc->startIndex = 0u;
		//	dc->baseVertex = 0u;
		//}

		//Keep track of the currently set vertex buffer, index buffer, cull state, alpha state, texture samplers, etc.
		//Whenever someone calls dc->Set<some>State(), simply change the IndexedDrawCall member for that state
		
		//For each Submit*() call, insert a new draw call into the queue.Our queue in this case would be raw memory, 
	    //and we would simply store the type of the operation(an indexed draw call), the key(used for sorting), 
	    //and all data that goes along with the draw call(in our case all the current states).After that, 
	    //we reset all our internal state members to their default value.

		
		std::shared_ptr<Jellyfish::iCamera> cam = m_CameraManager.GetCurrentCamera();
		if (cam == nullptr ) return;


		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, tf.position);
		modelMatrix = glm::scale(modelMatrix, tf.scale);

		modelMatrix = glm::rotate(modelMatrix, tf.rotation.x, glm::vec3(1.f, 0.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, tf.rotation.y, glm::vec3(0.f, 1.f, 0.f));
		modelMatrix = glm::rotate(modelMatrix, tf.rotation.z, glm::vec3(0.f, 0.f, 1.f));
		
		program.SetUniform("model", modelMatrix);

		model.model->AssignShaderToAllMeshes(program);
		model.model->Draw(); 
	}


	float NextTextPos(float prevPos)
	{
		int offset = DEBUG_TEXT_SIZE;
		return prevPos - offset;
	}

	void PostProcess() 
	{
		//Set up projection for post-process text rendering
		std::shared_ptr<Jellyfish::iCamera> cam = m_CameraManager.GetCurrentCamera();
		cam->SetOrthoProjection(m_windowSize.x, m_windowSize.y);
		glm::mat4 orthoProj = cam->GetProjection();

		//Render Text
		for (const auto &entity : textEntities) 
		{
			const RenderText &renderable{ entity.Get<const RenderText>() };
			gltr.Render(renderable.Text, renderable.Position, orthoProj, renderable.Color, renderable.Size);
		}

		float position;
		glm::vec3 camPos = cam->GetPosition();
		gltr.Render("FPS: " + std::to_string(1.f / Dt), { 0.f, position=NextTextPos(m_windowSize.y) }, orthoProj, { .5f,.8f,.2f });
		gltr.Render("Camera Pos X: " + std::to_string(camPos.x), { 0.f, position = NextTextPos(position) }, orthoProj, { 0.f, 0.f, 1.f });
		gltr.Render("Camera Pos Y: " + std::to_string(camPos.y), { 0.f, position = NextTextPos(position) }, orthoProj, { 0.f, 0.f, 1.f });
		gltr.Render("Camera Pos Z: " + std::to_string(camPos.z), { 0.f, position = NextTextPos(position) }, orthoProj, { 0.f, 0.f, 1.f });
	}

	void Draw() 
	{
		//TODO - tell renderer to draw everything in the renderqueues
		//currently the model's draw call happens in Process
	}

	void FrameEnd() 
	{
		//TODO

		//Upon a call to Sort(), we simply sort all the keys using e.g.a radix sort.
		//renderQueue::Sort();
		
		//Upon a call to Flush(), we walk the sorted array of operations, fetch the type, fetch the data, 
		//and call the respective render backend functions.It’s very similar to implementing a simple virtual machine.
		//renderQueue::Flush();

		
		Draw();
	}


	//Type Lists
	
	//Removing this one
	//EntitiesWith<const Jellyfish::iCamera> camEntities;
	
	EntitiesWith<const RenderText> textEntities;
	CameraManager m_CameraManager;

	//GL Impl 
	Jellyfish::GLText gltr{ "Text.sprog" };
	mutable Jellyfish::GLProgram program{ "Model.sprog" };

	//Other
	float Dt{ 0.f };
	
	glm::vec2 m_windowSize{ g_InitialWindowWidth, g_InitialWindowHeight };
};