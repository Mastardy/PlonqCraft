#include "Camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <print>

#include "glm/gtx/quaternion.hpp"

Camera::Camera(const glm::vec3& position, const float pitch, const float yaw)
{
	this->position = position;
	SetRotation(pitch, yaw);
	this->up = glm::vec3(0, 1, 0);
	this->right = -glm::normalize(glm::cross(forward, up));
}

void Camera::Move(const float x, const float y, const float z)
{
	Move(glm::vec3(x, y, z));
}

void Camera::Move(const glm::vec3& movement)
{
	position += forward * movement.z + glm::cross(forward, right) * movement.y + right * movement.x;
}

void Camera::SetRotation(const float pitch, const float yaw)
{
	this->pitch = glm::clamp(pitch, -90.0f, 90.0f);

	this->yaw = yaw + 90;
	this->yaw -= static_cast<float>(static_cast<int>(this->yaw / 360.0f)) * 360;

	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	forward = glm::normalize(front);
	up = glm::normalize(up);
	right = -glm::normalize(glm::cross(forward, up));
}

void Camera::Rotate(const float pitchOffset, const float yawOffset)
{
	this->pitch += pitchOffset;
	this->pitch = glm::clamp(this->pitch, -90.0f, 90.0f);

	this->yaw -= yawOffset;
	this->yaw -= static_cast<float>(static_cast<int>(this->yaw / 360.0f)) * 360;

	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	forward = glm::normalize(front);
	up = glm::normalize(up);
	right = -glm::normalize(glm::cross(forward, up));
}
