#pragma once

#include <glm/vec3.hpp>

#include "glm/fwd.hpp"
#include "glm/detail/type_quat.hpp"

class Camera
{
public:
	explicit Camera(const glm::vec3& position, float pitch, float yaw);

	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetForward() const { return forward; }

	void Move(float x, float y, float z);
	void Move(const glm::vec3& movement);

	void SetRotation(float pitch, float yaw);
	void Rotate(float pitchOffset, float yawOffset);

private:
	glm::vec3 position{};
	glm::vec3 forward{};
	glm::vec3 up{};
	glm::vec3 right{};
	float pitch{};
	float yaw{};
};
