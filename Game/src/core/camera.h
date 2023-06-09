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

    enum class Mode
    {
        Walk,
        Drone,
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

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom; // TODO : mouse sensitivity should react to zoom

    bool WantsJump = false; // This odesn't belong here, but again, time...
    bool CanJump = true;
    Mode Mode = Mode::Drone;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // TODO : The processing methods shouldn't be here I think. They should be in a class that controls the camera. 
    // The camera's job is to construct the view Matrix from its state.
    void ProcessKeyboard(MovementDirection direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    void LookAt(glm::vec3 target);
    void Lock();
    void Unlock();

    bool IsLocked() const { return m_allowMovement == false; }

private:
    void updateCameraVectors();
    bool m_allowMovement = true;
};
