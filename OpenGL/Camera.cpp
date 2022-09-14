#include "Camera.h"

Camera::Camera(float viewportWidth, float viewportHeight):  viewportWidth(viewportWidth), viewportHeight(viewportHeight) {
  this->position = glm::vec3(0.0f, 0.0f, 3.0f);
  this->front = glm::vec3(0.0f, 0.0f, -1.0f);
  this->up = glm::vec3(0.0f, 1.0f, 0.0f);
  this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  this->right = glm::vec3(1.0f, 0.0f, 0.0f);

  this->yaw = -90.0f;
  this->pitch = 0.0f;
  this->roll = 0.0f;

  this->fov = 45.0f;
  this->speed = 2.5f;
  this->sensitivity = 0.1f;

  this->updateVectors();
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
  float velocity = this->speed * deltaTime;

  switch (direction) {
    case FORWARD:
      this->position += this->front * velocity;
      break;
    case BACKWARD:
      this->position -= this->front * velocity;
      break;
    case LEFT:
      this->position -= glm::normalize(glm::cross(this->front, this->up)) * velocity;
      break;
    case RIGHT:
      this->position += glm::normalize(glm::cross(this->front, this->up)) * velocity;
      break;
    default:
      break;
  }

  this->updateVectors();
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
  float xSensitivity = this->sensitivity * xOffset;
  float ySensitivity = this->sensitivity * yOffset;

  this->yaw += xSensitivity;
  this->pitch += ySensitivity;

  if (constrainPitch) {
    if (this->pitch > 89.0f) {
      this->pitch = 89.0f;
    }
    if (this->pitch < -89.0f) {
      this->pitch = -89.0f;
    }
  }

  this->updateVectors();
}

void Camera::processMouseScroll(float yOffset) {
  if (this->fov >= 1.0f && this->fov <= 45.0f) {
    this->fov -= yOffset;
  }

  if (this->fov <= 1.0f) {
    this->fov = 1.0f;
  }

  if (this->fov >= 45.0f) {
    this->fov = 45.0f;
  }

  this->updateVectors();
}

void Camera::updateVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  front.y = sin(glm::radians(this->pitch));
  front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  this->front = glm::normalize(front);

  this->right = glm::normalize(glm::cross(this->front, this->worldUp));
  this->up = glm::normalize(glm::cross(this->right, this->front));

  this->view = glm::lookAt(this->position, this->position + this->front, this->up);
  this->projection = glm::perspective(glm::radians(this->fov), (float) this->viewportWidth / (float) this->viewportHeight, 0.1f, 100.0f);
}