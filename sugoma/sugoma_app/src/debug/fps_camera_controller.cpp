#include "fps_camera_controller.h"
#include "systems/input_system.h"

using namespace sugoma;
void FPSCameraController::update(float dt)
{
	glm::vec3 dir{};
	if(InputSystem::GetKey(KeyCode::A))
		dir.x -= 1;
	if (InputSystem::GetKey(KeyCode::D))
		dir.x += 1;
	if (InputSystem::GetKey(KeyCode::W))
		dir.z -= 1;
	if (InputSystem::GetKey(KeyCode::S))
		dir.z += 1;
	if(InputSystem::GetKey(KeyCode::E))
		dir.y += 1;
	if (InputSystem::GetKey(KeyCode::Q))
		dir.y -= 1;
	float s = speed;
	if(InputSystem::GetKey(KeyCode::LeftShift))
		s = sprintSpeed;
	dir *= s * dt;
	transform.position += transform.right() * dir.x + transform.forward() * dir.z + transform.up() * dir.y;

	if (m_focussed) {
		glm::vec2 mouseDelta = InputSystem::MouseDelta();

		transform.eulerAngles.x -= mouseDelta.y * sensitivity * dt;
		transform.eulerAngles.y -= mouseDelta.x * sensitivity * dt;
	}

	if (InputSystem::GetKeyDown(KeyCode::Escape)) { m_focussed = !m_focussed; InputSystem::SetCursorLockState(m_focussed); }
	
}
