#include "glad/include/glad.h"  // MUST be included BEFORE glfw3.h

#include "GLFW/glfw3.h"

#include "pilcrow/engine/core/RenderSystem.h"

Camera *cam;

void MouseCallback2(const Jellyfish::iWindow::EMouseMoved &mme) {
  if(mme.cursorData.buttonHeld[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS) {
    float       sensitivity{.07f};
    const float xoff{sensitivity * (mme.newPosition.x - mme.oldPosition.x)};
    const float yoff{sensitivity * (mme.newPosition.y - mme.oldPosition.y)};
    cam->yaw += xoff;
    cam->pitch -= yoff;
    if(cam->pitch > 89.9f) { cam->pitch = 89.9f; }
    if(cam->pitch < -89.9f) { cam->pitch = -89.9f; }
  }
}
// void ScrollCallback2(const GLFWWindow::EMouseScrolled &mse) {
//  if (cam->fov >= 1.0f && cam->fov <= 70.0f)
//    cam->fov -= (float)mse.offset.y;
//  if (cam->fov <= 1.0f)
//    cam->fov = 1.0f;
//  if (cam->fov >= 70.0f)
//    cam->fov = 70.0f;
//}

WindowManager::WindowManager() {
  // window.MouseMode(CursorMode::disabled);

  // window.On( ScrollCallback2 );
}

void WindowManager::Init(World &world) {
  // TODO(unknown): DX and other Window options
  Jellyfish::g_singleton_window = nullptr;
  Jellyfish::g_singleton_window = new Jellyfish::GLWindow;
  pWindow                       = Jellyfish::g_singleton_window;
  Jellyfish::g_singleton_window->CreateGameWindow(
    static_cast<unsigned>(m_windowSizeSetting.x),
    static_cast<unsigned>(m_windowSizeSetting.y), m_windowFullscreenSetting,
    "Welcome to Pilcrow Engine v0.0 ft. Jellyfish Renderer! :)");

  // Rebroadcast window events out to the engine
  pWindow->On([&](const Jellyfish::GLWindow::EWindowResized &event) {
    world.Emit(event);
  });

  pWindow->On([&](const Jellyfish::GLWindow::EWindowStateChanged &event) {
    world.Emit(event);
  });

  pWindow->On(
    [&](const Jellyfish::GLWindow::EKeyPressed &event) { world.Emit(event); });

  // Set callback functions
  pWindow->On(MouseCallback2);
}

void WindowManager::FrameStart() {
  cam = &Entities[0].Get<Camera>();
  this->ProcessInput(*cam);
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void WindowManager::FrameEnd() { pWindow->FrameEnd(); }

inline void WindowManager::ProcessInput(Camera &cam) {
  // TODO(unknown): fix camera
  float camSpeed{2.f * Dt};

  std::vector<int> keyarray;
  pWindow->PollInput(keyarray);

  for(int i : keyarray) {
    if(i == GLFW_KEY_ESCAPE) {
      pWindow->SetWindowState(Jellyfish::WindowState::closed);
    } else if(i == GLFW_KEY_1) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if(i == GLFW_KEY_2) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glLineWidth(1.5f);
    } else if(i == GLFW_KEY_3) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    } else if(i == GLFW_KEY_W) {
      cam.position += camSpeed * cam.Front();
    } else if(i == GLFW_KEY_S) {
      cam.position -= camSpeed * cam.Front();
    } else if(i == GLFW_KEY_A) {
      cam.position -= cam.Right() * camSpeed;
    } else if(i == GLFW_KEY_D) {
      cam.position += cam.Right() * camSpeed;
    } else if(i == GLFW_KEY_SPACE) {
      cam.LookAt({0.f, 0.f, 0.f});
    }
  }  // endfunc
}
