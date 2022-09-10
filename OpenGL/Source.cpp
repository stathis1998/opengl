#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

void proccessInput(GLFWwindow* window);

int main(int args, char** argv) {
	// Initializing glfw
	if (!glfwInit()) {
		std::cerr << "ERROR::GLFW::FAILED_TO_INITIALIZE" << std::endl;
		return -1;
	}

	// Adding window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	if (!window) {
		std::cerr << "ERROR::GLFW::FAILED_TO_CREATE_WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Setting window as current context
	glfwMakeContextCurrent(window);

	// Initializing glew
	if (glewInit() != GLEW_OK) {
		std::cerr << "ERROR::GLEW::FAILED_TO_INITIALIZE" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Setting viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Updating viewport
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	Shader shader("vertexShader.glsl", "fragmentShader.glsl");

	// Triangle vertices
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// Creating buffers
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Binding VAO
	glBindVertexArray(VAO);
	// Binding VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Setting vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	// Unbinding VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbinding VAO
	glBindVertexArray(0);

	// Using program
	shader.use();

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {
		// Checking for events
		glfwPollEvents();

		// Processing input
		proccessInput(window);

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Swapping buffers
		glfwSwapBuffers(window);
	}

	return 0;
}

void proccessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}