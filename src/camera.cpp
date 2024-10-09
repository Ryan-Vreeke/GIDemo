#include "camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>

Camera::Camera() : theta(0), phi(0) {}
Camera::Camera(glm::vec3 _pos, double _theta, double _phi)
    : phi(_phi), theta(_theta), pos(_pos) {}

void Camera::move(float x, float y) {
  pos += (right * x);
  pos += (forwards * y);
}

void Camera::look(float lookX, float lookY) {
  theta -= lookX;
  theta = std::fmod(theta, 360.0);
  phi = std::min(89.0, std::max(-89.0, (phi + lookY)));
}

void Camera::update() {
  forwards = glm::normalize(
      glm::vec3(glm::cos(glm::radians(phi)) * glm::sin(glm::radians(theta)),
                glm::sin(glm::radians(phi)),
                glm::cos(glm::radians(phi)) * glm::cos(glm::radians(theta))));

  right = glm::normalize(glm::cross(forwards, {0.0f, 1.0f, 0.0f}));
  up = glm::normalize(glm::cross(right, forwards));
  /*up = glm::vec3(0.0, 0.0, 1.0f);*/

  target = pos + forwards;
  lookMat = glm::lookAt(pos, target, up);
}

glm::mat4 Camera::getView() const { return lookMat; }
