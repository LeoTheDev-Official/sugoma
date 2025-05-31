#include "camera.h"

namespace sugoma::graphics 
{
	Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane) { MakePerspective(fov, aspectRatio, nearPlane, farPlane); }
	Camera::Camera(float size, float aspectRatio) { MakeOrthographic(size, aspectRatio); }
	void Camera::MakePerspective(float fov, float aspectRatio, float nearPlane, float farPlane) { m_projection = glm::perspectiveFov(glm::radians(fov), aspectRatio, 1.0f / aspectRatio, nearPlane, farPlane); }
	void Camera::MakeOrthographic(float size, float aspectRatio) { m_projection = glm::ortho(-size / 2, size / 2, -size / aspectRatio / 2, size / aspectRatio / 2); }
	glm::mat4 Camera::View() const { return glm::inverse(transform.matrix()); }
	glm::mat4 Camera::Projection() const { return m_projection; }
}