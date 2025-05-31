#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
namespace sugoma::core
{
	struct Transform 
	{
		glm::vec3 position;
		glm::vec3 eulerAngles;
		glm::vec3 scale = {1, 1, 1};

		glm::vec3 forward() const;
		glm::vec3 right() const;
		glm::vec3 up() const;

		glm::quat quaternion() const;

		glm::mat4 translation() const;
		glm::mat4 rotation() const;
		glm::mat4 scaling() const;

		glm::mat4 matrix() const;
	};
}