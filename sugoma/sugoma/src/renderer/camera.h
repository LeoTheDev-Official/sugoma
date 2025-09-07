#pragma once
#include <glm/glm.hpp>
#include "math/transform.h"
#include "textures/texture_cube.h"
namespace sugoma 
{
	using namespace core;
	using namespace graphics;
	enum class ClearMode
	{
		SolidColor,
		Skybox
	};
	class Camera 
	{
	public:
		Camera() = default;
	public:
		void MakeOrthographic(float aspectRatio, float size, float nearPlane = 0.1f, float farPlane = 1000.0f);
		void MakePerspective(float aspectRatio, float fovDeg, float nearPlane = 0.1f, float farPlane = 1000.0f);
		
		const glm::mat4& Projection() const;
		glm::mat4 View() const;
	public:
		static Camera Orthographic(float aspectRatio, float size, float nearPlane = 0.1f, float farPlane = 1000.0f);
		static Camera Perspective(float aspectRatio, float fovDeg, float nearPlane = 0.1f, float farPlane = 1000.0f);
	public:
		Transform transform{};
		Ref<TextureCube> skybox{};
		glm::vec4 solidColor = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
		ClearMode clearMode = ClearMode::SolidColor;
	private:
		glm::mat4 m_projectionMatrix = glm::mat4(1);
	};
}