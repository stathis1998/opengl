#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

void proccessInput(GLFWwindow* window);

int main(int args, char** argv) {
	// Initializing glfw
	if (!glfwInit()) {
		std::cerr << "Failed to initialize glfw" << std::endl;
		return -1;
	}

	// Adding window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Setting window as current context
	glfwMakeContextCurrent(window);

	// Initializing glew
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize glew" << std::endl;
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

	// Vertex shader code
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\n\0";

	// Creating vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attaching shader code to shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	// Compiling shader
	glCompileShader(vertexShader);
	// Checking for errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "Failed to compile vertex shader" << std::endl;
		std::cerr << infoLog << std::endl;
	}

	// Fragment shader code
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	// Creating fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attaching shader code to shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	// Compiling shader
	glCompileShader(fragmentShader);
	// Checking for errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "Failed to compile fragment shader" << std::endl;
		std::cerr << infoLog << std::endl;
	}

	// Creating shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// Attaching shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Linking program
	glLinkProgram(shaderProgram);
	// Checking for errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Failed to link shader program" << std::endl;
		std::cerr << infoLog << std::endl;
	}

	// Deleting shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Triangle vertices
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	VertexArray* VAO = new VertexArray();
	VertexBuffer* VBO = new VertexBuffer(vertices, sizeof(vertices));
	ElementBuffer* EBO = new ElementBuffer(indices, sizeof(indices));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	VAO->unbind();
	VBO->unbind();
	EBO->unbind();

	// Using program
	glUseProgram(shaderProgram);

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