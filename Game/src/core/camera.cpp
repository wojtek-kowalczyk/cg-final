#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: Forward(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Forward, Up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	// TODO : should camera handle this?
	//return glm::perspective(glm::radians(Zoom), (float)Consts::SCREEN_WIDTH / (float)Consts::SCREEN_HEIGHT, 0.1f, 100.0f)
	return glm::mat4(1.0f);
}

void Camera::LookAt(glm::vec3 target)
{
	glm::vec3 newFront = glm::normalize(target - Position); // TODO : front should Actually be called forward in my opinion
	Pitch = glm::degrees(asinf(newFront.z));
	Yaw = glm::degrees(atan2f(newFront.y, newFront.x));
	updateCameraVectors();
}

void Camera::Lock()
{
	m_allowMovement = false;
}

void Camera::Unlock()
{
	m_allowMovement = true;
}

void Camera::ProcessKeyboard(MovementDirection direction, float deltaTime)
{
	if (m_allowMovement == false)
		return;

	float velocity = MovementSpeed * deltaTime;
	if (direction == MovementDirection::Forward)
		Position += Forward * velocity;
	if (direction == MovementDirection::Backward)
		Position -= Forward * velocity;
	if (direction == MovementDirection::Left)
		Position -= Right * velocity;
	if (direction == MovementDirection::Right)
		Position += Right * velocity;

	// Up Down are world space movements.
	if (direction == MovementDirection::Up)
		Position.y += velocity;
	if (direction == MovementDirection::Down)
		Position.y -= velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	if (m_allowMovement == false)
		return;

	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (m_allowMovement == false)
		return;

	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// TODO : extract into transform component?
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Forward = glm::normalize(front);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}
