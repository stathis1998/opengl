#include "Renderer.h"

Renderer::Renderer() {

}

Renderer* Renderer::getInstance() {
	if (instance == nullptr) {
		instance = new Renderer();
	}

	return instance;
}

void Renderer::render() {

}