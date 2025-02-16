#include "Camera.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera:: Camera(glm::vec3 CameraPos,
    glm::vec3 CameraUp,
    glm::vec3 CameraFront,float yaw,float pitch)
{
    this->Position = CameraPos;
    this->Up = CameraUp;
    this->Front = CameraFront;
    this->yaw = 0.0f;
    this->pitch = 90.0f;
    this->MouseSensitivity = 0.1f;
    this->MovementSpeed =3.5f;
    UpdateCameraVectors();
}

glm::mat4 Camera :: GetViewMatrix()
{
    return glm::lookAt(Position,Position + Front,Up);
}
glm::vec3 Camera :: GetCameraPos()
{
    return Position;
}
glm::vec3 Camera :: GetCameraFront()
{
    return Front;
}
void Camera::ProcessWASD(CAMERA_MOVEMENT mov, float deltaTime)
{
    float cameraSpeed = MovementSpeed*deltaTime;
    if (mov == CAMERA_FORWARD)
        Position += Front * cameraSpeed;
    if (mov == CAMERA_BACKWARD)
        Position -= Front * cameraSpeed;
    if (mov == CAMERA_LEFT)
        Position -= Right * cameraSpeed;
    if (mov == CAMERA_RIGHT)
        Position += Right * cameraSpeed;
}
void Camera::SetZoom(float scrolldy)
{
    Zoom += scrolldy;
    if(Zoom > 45.0f) Zoom = 45.0f;
    else if(Zoom < 1.0f) Zoom = 1.0f;

}

void Camera::ProcessMouse(float dx,float dy)
{
   dx *= MouseSensitivity;
   dy *= MouseSensitivity;

   yaw += dx;
   pitch += dy;
   if (pitch > 89.0f)
       pitch = 89.0f;
   if (pitch < -89.0f)
       pitch = -89.0f;
   UpdateCameraVectors();
}
void Camera::UpdateCameraVectors()
{
   glm::vec3 newFront;
   newFront.x = glm::cos(glm::radians(yaw))*glm::cos(glm::radians(pitch));
   newFront.y = glm::sin(glm::radians(pitch));
   newFront.z = glm::sin(glm::radians(yaw))*glm::cos(glm::radians(pitch));
   Front = glm::normalize(newFront);
   Right = glm::normalize(glm::cross(Front,WorldUp));
   Up = glm::normalize(glm::cross(Right,Front));
}