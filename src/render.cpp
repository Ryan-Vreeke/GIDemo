#include "render.h"
#include <GL/gl.h>
#include <cstdio>
#include <glm/ext.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sys/types.h>

Render::Render() {
  program.BuildFiles("../shaders/vert.glsl", "../shaders/frag.glsl");

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // create a VBO
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // setup texture

  glGenFramebuffers(1, &fbo);

  glGenTextures(1, &id_texture);
  glBindTexture(GL_TEXTURE_2D, id_texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 400, 400, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id_texture, 0);

  GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  // Check framebuffer completeness
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("Framebuffer not complete!\n");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  id_res = glGetUniformLocation(program.GetID(), "resolution");
  id_color = glGetUniformLocation(program.GetID(), "drawColor");
  id_to = glGetUniformLocation(program.GetID(), "to");
  id_from = glGetUniformLocation(program.GetID(), "from");
  id_drawing = glGetUniformLocation(program.GetID(), "drawing");
}

void Render::update(std::vector<char> texture) {
  glBindTexture(GL_TEXTURE_2D, id_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 400, 400, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data());
  glBindTexture(GL_TEXTURE_2D, 0);
}


void Render::draw() {
  GLint origFB;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &origFB);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id_texture);
  glUniform1i(id_texture, 0);

  program.Bind();

  // first draw
  glUniform4fv(id_color, 1, glm::value_ptr(lightColor));
  glUniform2fv(id_to, 1, glm::value_ptr(to));
  glUniform2fv(id_from, 1, glm::value_ptr(from));
  glUniform1i(id_drawing, isDrawing);
  glUniform1i(id_res, 400);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  //second
  glBindFramebuffer(GL_FRAMEBUFFER, origFB);
  
  glUniform1i(id_drawing, false);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}
