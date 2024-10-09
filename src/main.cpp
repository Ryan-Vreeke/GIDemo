#include "render.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdio>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

void update();
void render();
void processInput();
void updateFps(double);

// controls
void keyPress(GLFWwindow *window, int key, int scancode, int action, int mods);
void scrollCallback(GLFWwindow *window, double x, double y);
void mouseMove(GLFWwindow *glfw, double x, double y);
void mousePress(GLFWwindow *glfw, int key, int action, int mods);

constexpr double frameRate = 60.0;
constexpr double frameDuration = 1000 / frameRate;
constexpr bool fps = false;

double deltaTime = 0;
double prevTime = 0;
int frameCount = 0;

std::unique_ptr<nWindow> window;
std::unique_ptr<Render> renderer;
std::vector<char> world;


int color = 0;
std::vector<glm::vec4> colors;

int main(void) {
  colors.push_back(glm::vec4(0.820, 0.118, 0.565, 1.0));
  colors.push_back(glm::vec4(1.000, 0.675, 0.000, 1.0));
  colors.push_back(glm::vec4(0.110, 1.000, 0.000, 1.0));
  colors.push_back(glm::vec4(0.0));
  colors.push_back(glm::vec4(1.0));

  window = std::make_unique<nWindow>(400, 400);
  window->init_window();
  window->keyCallback(keyPress);
  window->mousePassiveCallback(mouseMove);
  window->mouseActiveCallback(mousePress);

  renderer = std::make_unique<Render>();

  double lastTime = glfwGetTime();
  while (window->isOpen()) {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;

    render();

    lastTime = currentTime;
    double frameTime = glfwGetTime() - lastTime;
    double sleepTime = frameDuration - frameTime * 1000;
    if (sleepTime > 0) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<int>(sleepTime)));
    }

    if (fps)
      updateFps(currentTime);

    window->redraw();
  }
}

void update() {}

void render() {}

void updateFps(double currentTime) {
  frameCount++;
  if (currentTime - prevTime >= 1.0) {
    std::cout << "FPS: " << frameCount << std::endl;
    frameCount = 0;
    prevTime = currentTime;
  }
}


void keyPress(GLFWwindow *glfw, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    window->close();
  }

  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
    if (color == 4) {
      color = 0;
      return;
    }
    color++;
  } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
    if (color == 0) {
      color = 4;
      return;
    }
    color--;
  }
  std::cout << color << std::endl;

}

bool firstMove = true;
float lastX;
float lastY;

void mousePress(GLFWwindow *glfw, int key, int action, int mods) {
  if (key != GLFW_MOUSE_BUTTON_1)
    return;

  if (action == GLFW_PRESS) {
    renderer->isDrawing = true;
  } else if (action == GLFW_RELEASE) {
    renderer->isDrawing = false;
  }

  double x, y;
  glfwGetCursorPos(glfw, &x, &y);
  renderer->from = glm::vec2(x, y);
  renderer->lightColor = colors[color];

  lastX = x;
  lastY = y;
}

void mouseMove(GLFWwindow *glfw, double x, double y) {

  if (renderer->isDrawing) {
    renderer->to = glm::vec2(x, 400 - y);
    renderer->from = glm::vec2(lastX, 400 - lastY);
    renderer->draw();
  }

  lastX = x;
  lastY = y;
}
