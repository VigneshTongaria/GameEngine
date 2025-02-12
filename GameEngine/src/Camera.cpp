#include "Camera.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera:: Camera(glm::vec3 CameraPos = glm::vec3(0.0f,0.0f,0.0f),
    glm::vec3 CameraUp = glm::vec3(0.0f,1.0f,0.0f),
    glm::vec3 CameraFront = glm::vec3(0.0f,0.0f,-1.0f),float yaw = 0.0f,float pitch = 90.0f)
{
    Position = CameraPos;
    Up = CameraUp;
    Front = CameraFront;
    yaw = 0.0f;
    pitch = 90.0f;
    UpdateCameraVectors();
}

glm::mat4 Camera :: GetViewMattrix()
{
    return glm::lookAt(Position,Position + Front,Up);
}
void Camera::ProcessWASD(CAMERA_MOVEMENT mov, float deltaTime)
{
    float cameraSpeed = MovementSpeed*deltaTime;
    if (mov == FORWARD)
        Position += Front * cameraSpeed;
    if (mov == BACKWARD)
        Position -= Front * cameraSpeed;
    if (mov == LEFT)
        Position -= Right * cameraSpeed;
    if (mov == RIGHT)
        Position += Right * cameraSpeed;
}
void Camera::SetZoom(float scrolldy)
{
    Zoom += scrolldy;
    if(Zoom >= 45.0f) Zoom = 45.0f;
    else if(Zoom <= 1.0f) Zoom = 1.0f;

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
   newFront.x = glm::cos(yaw)*glm::cos(pitch);
   newFront.y = glm::sin(pitch);
   newFront.z = glm::sin(yaw)*glm::cos(pitch);
   Front = glm::normalize(newFront);
   Right = glm::normalize(glm::cross(Front,WorldUp));
   Up = glm::normalize(glm::cross(Right,Front));
}