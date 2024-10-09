#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>

class Camera{
public:
  glm::vec3 pos;
  glm::vec3 forwards;
  glm::vec3 target;
  glm::vec3 right;
  glm::vec3 up;
  double theta;
  double phi;

  Camera();
  Camera(glm::vec3, double, double);

  void move(float x, float y);
  void look(float lookX, float lookY);
  void update();
  glm::mat4 getView() const;

private:
  glm::mat4 lookMat;
};
