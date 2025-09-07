#include "transform.h"

namespace sugoma::core
{
	glm::vec3 Transform::forward() const { return glm::vec3(rotation() * glm::vec4(0, 0, 1, 0)); }
	glm::vec3 Transform::right() const { return glm::vec3(rotation() * glm::vec4(1, 0, 0, 0)); }
	glm::vec3 Transform::up() const { return glm::vec3(rotation() * glm::vec4(0, 1, 0, 0)); }
	glm::quat Transform::quaternion() const { return glm::quat(glm::radians(eulerAngles)); }
	glm::mat4 Transform::translation() const { return glm::translate(glm::mat4(1), position); }
	glm::mat4 Transform::rotation() const { return glm::toMat4(quaternion()); }
	glm::mat4 Transform::scaling() const { return glm::scale(glm::mat4(1), scale); }
	glm::mat4 Transform::matrix() const { return translation() * scaling() * rotation(); }
}