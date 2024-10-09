#pragma once

#include "camera.h"
#include <GL/glew.h>
#include <cyGL.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

inline const float positions[] = {
    -1.0f, -1.0f, // Bottom left
    1.0f,  -1.0f, // Bottom right
    1.0f,  1.0f,  // Top right
    -1.0f, -1.0f, // Bottom left (again)
    1.0f,  1.0f,  // Top right (again)
    -1.0f, 1.0f   // Top left
};

class Render {
public:
  GLuint buffer;
  GLuint VAO;
  cy::GLSLProgram program;

  glm::vec4 lightColor;

  // drawing stuff
  glm::vec2 to;
  glm::vec2 from;
  bool isDrawing = false;

  Render();
  void update(std::vector<char>);
  void draw();

private:
  GLuint id_texture, id_res, id_color, id_to, id_from, id_drawing, fbo;
};
