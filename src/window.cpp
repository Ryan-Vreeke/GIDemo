#include "window.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdio>

nWindow::nWindow(uint16_t width, uint16_t height)
    : m_width(width), m_height(height) {}

nWindow::~nWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int nWindow::init_window() {
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

  glfwWindowHint(GLFW_RED_BITS, 8); // Set RGBA color depth
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24); // Set depth buffer size
                                       //
  window = glfwCreateWindow(m_width, m_height, "Radiance Cascades", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -2;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  /*glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    return -3;
  }

  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  open = true;
  return 0;
}

void nWindow::redraw() {
  glfwPollEvents();
  glfwSwapBuffers(window);
}

void nWindow::mousePassiveCallback(void (*passive)(GLFWwindow *, double,
                                                   double)) {
  glfwSetCursorPosCallback(window, passive);
}

void nWindow::mouseActiveCallback(void (*cb)(GLFWwindow *, int, int, int)) {
  glfwSetMouseButtonCallback(window, cb);
}

void nWindow::scrollCallback(void (*cb)(GLFWwindow *, double, double)) {
  glfwSetScrollCallback(window, cb);
}

void nWindow::keyCallback(void (*cb)(GLFWwindow *, int, int, int, int)) {
  glfwSetKeyCallback(window, cb);
}

bool nWindow::isOpen() { return open; }

void nWindow::close() {
  open = false;
  glfwSetWindowShouldClose(window, true);
}
