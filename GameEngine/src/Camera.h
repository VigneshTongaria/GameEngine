#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_MOVEMENT{
    CAMERA_FORWARD,CAMERA_RIGHT,CAMERA_LEFT,CAMERA_BACKWARD,
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
  float MouseSensitivity;
  float perspectiveAngle;
  float height;
  float width;
  float nearPlane;
  float farPlane;
  Camera(glm::vec3 CameraPos = glm::vec3(0.0f,0.0f,3.0f),
  glm::vec3 CameraUp = glm::vec3(0.0f,1.0f,0.0f),
  glm::vec3 CameraFront = glm::vec3(0.0f,0.0f,1.0f),float yaw = 0.0f,float pitch = 89.0f,
  float perspectiveAnlge = glm::radians(60.0f),float height = 800.0f,float width = 600.0f,
  float nearPlane = 0.1f,float farPlane = 100.0f);

  glm::mat4 GetViewMatrix();
  glm::mat4 GetProjectionMatrix();
  glm::vec3 GetCameraPos();
  glm::vec3 GetCameraFront();
  void ProcessWASD(CAMERA_MOVEMENT mov, float deltaTime);
  void ProcessMouse(float dx, float dy);
  void SetZoom(float scrolldy);
};
//extern Camera MainCamera;
#endif
