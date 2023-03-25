#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    enum class MovementDirection
    {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down,
    };

    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Forward; // this should be readonly. changing this does not affect pitch and yaw, and the next update overrides this value with one computed from pitch an yaw.
    glm::vec3 Up; // the same as above
    glm::vec3 Right; // the same as above
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;
    // We don't do roll cuz its more complicated and I don't need it.

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom; // TODO : mouse sensitivity should react to zoom

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // TODO : The processing methods shouldn't be here I think. They should be in a class that controls the camera. The camera's job is to construct the view Matrix from its state.
    void ProcessKeyboard(MovementDirection direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    glm::mat4 GetViewMatrix() const;
    void LookAt(glm::vec3 target);

private:
    void updateCameraVectors();
};
