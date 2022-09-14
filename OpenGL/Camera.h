#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class CameraMovement {
	UP,
	DOWN,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	unsigned int viewportWidth;
	unsigned int viewportHeight;

	float near;
	float far;

	float yaw;
	float pitch;
	float roll;

	float fov;
	float speed;
	float sensitivity;

	glm::mat4 view;
	glm::mat4 projection;
public:
	Camera(unsigned int viewportWidth, unsigned int viewportHeight);

	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void processMouseScroll(float yOffset);

	void updateVectors();

	// Getters
	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3 getFront() const { return this->front; }
	inline glm::vec3 getUp() const { return this->up; }
	inline glm::vec3 getRight() const { return this->right; }
	inline glm::vec3 getWorldUp() const { return this->worldUp; }

	inline unsigned int getViewportWidth() const { return this->viewportWidth; }
	inline unsigned int getViewportHeight() const { return this->viewportHeight; }

	inline float getNear() const { return this->near; }
	inline float getFar() const { return this->far; }

	inline float getYaw() const { return this->yaw; }
	inline float getPitch() const { return this->pitch; }
	inline float getRoll() const { return this->roll; }

	inline float getFov() const { return this->fov; }
	inline float getSpeed() const { return this->speed; }
	inline float getSensitivity() const { return this->sensitivity; }

	inline glm::mat4 getView() const { return this->view; }
	inline glm::mat4 getProjection() const { return this->projection; }

	// Setters
	inline void setPosition(glm::vec3 position) { this->position = position; }
	inline void setFront(glm::vec3 front) { this->front = front; }
	inline void setUp(glm::vec3 up) { this->up = up; }
	inline void setRight(glm::vec3 right) { this->right = right; }
	inline void setWorldUp(glm::vec3 worldUp) { this->worldUp = worldUp; }

	inline void setViewportWidth(unsigned int viewportWidth) { this->viewportWidth = viewportWidth; }
	inline void setViewportHeight(unsigned int viewportHeight) { this->viewportHeight = viewportHeight; }

	inline void setNear(float near) { this->near = near; }
	inline void setFar(float far) { this->far = far; }

	inline void setYaw(float yaw) { this->yaw = yaw; }
	inline void setPitch(float pitch) { this->pitch = pitch; }
	inline void setRoll(float roll) { this->roll = roll; }

	inline void setFov(float fov) { this->fov = fov; }
	inline void setSpeed(float speed) { this->speed = speed; }
	inline void setSensitivity(float sensitivity) { this->sensitivity = sensitivity; }

	inline void setView(glm::mat4 view) { this->view = view; }
	inline void setProjection(glm::mat4 projection) { this->projection = projection; }
};

