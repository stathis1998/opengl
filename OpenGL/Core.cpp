#include "Core.h"

#include <iostream>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Texture.h"

Core* Core::instance = nullptr;

Core::Core(): window(nullptr), camera(nullptr), firstMouse(true), lastX(0.0f), lastY(0.0f), deltaTime(0.0f), lastFrame(0.0f) {}

Core::~Core() {
	// Clean up
	delete this->camera;

	// Terminating glfw
	glfwTerminate();
}

Core* Core::getInstance() {
	if (instance == nullptr) {
		instance = new Core();
	}
	return instance;
}

void Core::init(const char* title, int width, int height) {
	// Camera
	this->firstMouse = true;
	this->lastX = width / 2.0f;
	this->lastY = height / 2.0f;

	// Timing
	this->deltaTime = 0.0f;
	this->lastFrame = 0.0f;

	// Initializing glfw
	if (!glfwInit()) {
		std::cerr << "ERROR::GLFW::FAILED_TO_INITIALIZE" << std::endl;
		return;
	}

	// Adding window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);
	if (!this->window) {
		std::cerr << "ERROR::GLFW::FAILED_TO_CREATE_WINDOW" << std::endl;
		glfwTerminate();
		return;
	}

	// Setting window as current context
	glfwMakeContextCurrent(this->window);

	// Initializing glew
	if (glewInit() != GLEW_OK) {
		std::cerr << "ERROR::GLEW::FAILED_TO_INITIALIZE" << std::endl;
		glfwTerminate();
		return;
	}

	// Setting viewport
	int w, h;
	glfwGetFramebufferSize(this->window, &w, &h);
	glViewport(0, 0, w, h);

	// Updating viewport
	glfwSetFramebufferSizeCallback(this->window, this->frameBufferSizeCallback);
	// Callbacks for mouse input
	glfwSetCursorPosCallback(this->window, this->mouseCallback);
	glfwSetScrollCallback(this->window, this->scrollCallback);

	// Initializing camera
	this->camera = new Camera(width, height);

	// Depth test
	glEnable(GL_DEPTH_TEST);

	// Capture mouse input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Window user pointer
	glfwSetWindowUserPointer(this->window, this);
}

void Core::run() {
	Shader shader("vertexShader.glsl", "fragmentShader.glsl");

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

	VertexArray VAO;
	VertexBuffer VBO(vertices, sizeof(vertices));

	// Position
	VAO.addAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	// Texture
	VAO.addAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));

	VAO.unbind();
	VBO.unbind();

	Texture texture("assets/container.jpg");
	shader.use();

	while (!glfwWindowShouldClose(this->window)) {
		// Timing
		float currentFrame = static_cast<float>(glfwGetTime());
		this->deltaTime = currentFrame - this->lastFrame;
		this->lastFrame = currentFrame;
		// Checking for events
		glfwPollEvents();
		// Proccess input
		this->handleEvents();
		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Set uniforms
		shader.setMat4("view", this->camera->getView());
		shader.setMat4("projection", this->camera->getProjection());
		// Draw all the cubes
		texture.bind();
		VAO.bind();

		for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++) {
			// Model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		VAO.unbind();
		texture.unbind();
		// Swapping buffers
		glfwSwapBuffers(window);
	}
}

void Core::handleEvents() {
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		this->camera->processKeyboard(CameraMovement::UP, deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		this->camera->processKeyboard(CameraMovement::DOWN, deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		this->camera->processKeyboard(CameraMovement::FORWARD, deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		this->camera->processKeyboard(CameraMovement::BACKWARD, deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		this->camera->processKeyboard(CameraMovement::LEFT, deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->camera->processKeyboard(CameraMovement::RIGHT, deltaTime);
	}

	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(this->window, true);
	}
}

void Core::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Core::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	Core* core = static_cast<Core*>(glfwGetWindowUserPointer(window));

	float xPosition = static_cast<float>(xPos);
	float yPosition = static_cast<float>(yPos);

	if (core->getFirstMouse()) {
		core->setLastX(xPosition);
		core->setLastY(yPosition);
		core->setFirstMouse(false);
	}

	float xOffset = xPosition - core->getLastX();
	float yOffset = core->getLastY() - yPosition;

	core->setLastX(xPosition);
	core->setLastY(yPosition);

	core->getCamera()->processMouseMovement(xOffset, yOffset);
}

void Core::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	Core* core = static_cast<Core*>(glfwGetWindowUserPointer(window));
	core->getCamera()->processMouseScroll(static_cast<float>(yOffset));
}