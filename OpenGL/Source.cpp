#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

int main(int args, char** argv) {
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "ERROR::GLFW::INIT_FAILED" << std::endl;
		return -1;
	}

	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	if (!window) {
		std::cerr << "ERROR::GLFW::WINDOW_FAILED" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set context
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "ERROR::GLEW::INIT_FAILED" << std::endl;
		return -1;
	}

	// Set glew experimental
	glewExperimental = GL_TRUE;

	// Set viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Set clear color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Shader
	Shader shader("shader.vert", "shader.frag");

	// Creating a triangle - Sample Code
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// Create a vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create a vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Poll events
		glfwPollEvents();

		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers
		glfwSwapBuffers(window);
	}

	return 0;
}