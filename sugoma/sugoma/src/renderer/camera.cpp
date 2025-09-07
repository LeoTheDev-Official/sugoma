#include "camera.h"

namespace sugoma 
{
	void Camera::MakeOrthographic(float aspectRatio, float size, float nearPlane, float farPlane)
	{
		float hw = aspectRatio / 2.0f * size;
		float hh = size / 2.0f;
		m_projectionMatrix = glm::orthoRH(-hw, hw, -hh, hh, nearPlane, farPlane);
	}
	void Camera::MakePerspective(float aspectRatio, float fovDeg, float nearPlane, float farPlane)
	{
		m_projectionMatrix = glm::perspectiveFovRH(glm::radians(fovDeg), aspectRatio, 1.0f, nearPlane, farPlane);
	}
	const glm::mat4& Camera::Projection() const { return m_projectionMatrix; }
	glm::mat4 Camera::View() const { return glm::inverse(transform.matrix()); }
	Camera Camera::Orthographic(float aspectRatio, float size, float nearPlane, float farPlane)
	{
		Camera c{};
		c.MakeOrthographic(aspectRatio, size, nearPlane, farPlane);
		return c;
	}
	Camera Camera::Perspective(float aspectRatio, float fovDeg, float nearPlane, float farPlane)
	{
		Camera c{};
		c.MakePerspective(aspectRatio, fovDeg, nearPlane, farPlane);
		return c;
	}
}