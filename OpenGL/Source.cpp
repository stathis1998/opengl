#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
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

	// Box vertices
	float vertices[] = {
		0.5f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	 -0.5f, -0.5f, 0.0f,
	 -0.5f,  0.5f, 0.0f
	};

	// Indices
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	VertexArray* VAO = new VertexArray();
	VertexBuffer* VBO = new VertexBuffer(vertices, sizeof(vertices));
	ElementBuffer* EBO = new ElementBuffer(indices, sizeof(indices));

	VAO->addAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	VAO->unbind();
	VBO->unbind();
	EBO->unbind();

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
		VAO->bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		VAO->unbind();

		// Swapping buffers
		glfwSwapBuffers(window);
	}

	delete VAO;
	delete VBO;
	delete EBO;

	// Terminating glfw
	glfwTerminate();

	return 0;
}

void proccessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}