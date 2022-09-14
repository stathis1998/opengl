#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"

class Core {
private:
	static Core* instance;

	GLFWwindow* window;

	// Camera
	Camera* camera;
	bool firstMouse;
	float lastX, lastY;

	// Timing
	float deltaTime, lastFrame;

	Core();
	~Core();

	void handleEvents();
public:
	static Core* getInstance();

	void init(const char* title, int width, int height);
	void run();

	Core(Core const&) = delete;
	void operator=(Core const&) = delete;

	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	// Getters
	inline Camera* getCamera() { return this->camera; };
	inline bool getFirstMouse() { return this->firstMouse; };
	inline float getLastX() { return this->lastX; };
	inline float getLastY() { return this->lastY; };

	//// Setters
	inline void setFirstMouse(bool firstMouse) { this->firstMouse = firstMouse; };
	inline void setLastX(float lastX) { this->lastX = lastX; };
	inline void setLastY(float lastY) { this->lastY = lastY; };
};