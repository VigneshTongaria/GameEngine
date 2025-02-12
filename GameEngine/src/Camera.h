#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_MOVEMENT{
    FORWARD,RIGHT,LEFT,BACKWARD,
};

class Camera
{
private:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp = glm::vec3(0.0f,1.0f,0.0f);

  void UpdateCameraVectors();

public:
  float yaw;
  float pitch;
  float MovementSpeed;
  float Zoom;
  float MouseSensitivity = 0.1f;
  Camera(glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f),
         glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f), float yaw, float pitch);

  glm::mat4 GetViewMattrix();
  void ProcessWASD(CAMERA_MOVEMENT mov, float deltaTime);
  void ProcessMouse(float dx, float dy);
  void SetZoom(float scrolldy);
};
#endif
