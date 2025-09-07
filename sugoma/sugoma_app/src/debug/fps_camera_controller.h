#pragma once
#include "math/transform.h"

class FPSCameraController
{
public:
	FPSCameraController() = default;
	sugoma::core::Transform transform{};
	float speed = 5.0f;
	float sprintSpeed = 10.0f;
	float sensitivity = 10.0f;

	void update(float dt);

private:
	bool m_focussed = false;
};