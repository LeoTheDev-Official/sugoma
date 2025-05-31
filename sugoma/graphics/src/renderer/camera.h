#pragma once
#include "math/transform.h"
#include "textures/texture_cube.h"
namespace sugoma::graphics 
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(float fov, float aspectRatio, float nearPlane, float farPlane);
		Camera(float size, float aspectRatio);

		void MakePerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
		void MakeOrthographic(float size, float aspectRatio);

		glm::mat4 View() const;
		glm::mat4 Projection() const;
	public:
		core::Transform transform;
		Ref<TextureCube> skybox;
		glm::vec4 clearColor;
	private:
		glm::mat4 m_projection;
	};
}