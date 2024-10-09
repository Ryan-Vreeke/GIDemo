#pragma once

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdint>

class nWindow {
public:
  nWindow(uint16_t width, uint16_t height);
  ~nWindow();

  int init_window();
  void redraw();
  void mousePassiveCallback(void (*passive)(GLFWwindow *, double, double));
  void mouseActiveCallback(void (*cb)(GLFWwindow *, int, int, int));
  void scrollCallback(void (*cb)(GLFWwindow *, double, double));
  void keyCallback(void (*cb)(GLFWwindow *, int, int, int, int));
  bool isOpen();
  void close();

private:
  uint16_t m_width;
  uint16_t m_height;
  GLFWwindow *window;
  bool open;

};
