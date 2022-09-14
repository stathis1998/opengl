#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"

void proccessInput(GLFWwindow* window);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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

	Shader* shader = new Shader("vertexShader.glsl", "fragmentShader.glsl");

	float vertices[] = {
		// Position           // Texture
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	VertexArray* VAO = new VertexArray();
	VertexBuffer* VBO = new VertexBuffer(vertices, sizeof(vertices));

	// Position
	VAO->addAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	// Texture
	VAO->addAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));

	VAO->unbind();
	VBO->unbind();

	// Creating texture1
	unsigned int texture1;
	glGenTextures(1, &texture1);
	// Binding texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set the texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Flip image
	stbi_set_flip_vertically_on_load(true);

	// Load image
	int widthImage, heightImage, nrChannels;
	unsigned char* data = stbi_load("assets/container.jpg", &widthImage, &heightImage, &nrChannels, 0);

	// Check if image is loaded
	if (data) {
		// Generating texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "ERROR::STB_IMAGE::FAILED_TO_LOAD_IMAGE" << std::endl;
	}

	// Free image data
	stbi_image_free(data);

	// Creating texture2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	// Binding texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set the texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image
	data = stbi_load("assets/awesomeface.png", &widthImage, &heightImage, &nrChannels, 0);
	if (data) {
		// Generating texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "ERROR::STB_IMAGE::FAILED_TO_LOAD_IMAGE" << std::endl;
	}

	// Free image data
	stbi_image_free(data);

	// Using shader program
	shader->use();
	shader->setInt("texture2", 1);

	// Depth test
	glEnable(GL_DEPTH_TEST);

	// Capture mouse input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set mouse callback
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		// First mouse
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		// Get mouse position
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		// Calculate mouse sensitivity
		float mouseSensitivity = 0.1f;
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		// Calculate camera rotation
		yaw += xoffset;
		pitch += yoffset;

		// Limit pitch
		if (pitch > 89.0f) {
			pitch = 89.0f;
		} else if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		// Calculate camera direction
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	});

	// Set scrollbar callback
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
		// Calculate field of view
		fov -= (float) yoffset;
		if (fov < 1.0f) {
			fov = 1.0f;
		} else if (fov > 45.0f) {
			fov = 45.0f;
		}
	});

	// Rendering loop
	while (!glfwWindowShouldClose(window)) {
		// Next frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Checking for events
		glfwPollEvents();

		// Processing input
		proccessInput(window);

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind textures and activate them
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// View
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// Projection
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), (float) width / (float) height, 0.1f, 100.0f);

		// Set uniforms
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);

		// Draw all the cubes
		VAO->bind();
		for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++) {
			// Model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		VAO->unbind();

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Swapping buffers
		glfwSwapBuffers(window);
	}

	// Deleting buffers
	delete VAO;
	delete VBO;

	// Deleting shader
	delete shader;

	// Terminating glfw
	glfwTerminate();

	return 0;
}

void proccessInput(GLFWwindow* window) {
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}